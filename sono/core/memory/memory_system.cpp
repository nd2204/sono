#include "memory_system.h"
#include "../common/snassert.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

DEFINE_STRING_CONSTANTS(kAllocationTypeStr, __FOREACH_ALLOCATION_TYPES);

template <>
MemorySystem *Singleton<MemorySystem>::m_sInstance = nullptr;

MemorySystem::MemorySystem()
  : m_TotalAllocated(0)
  , m_TotalFreed(0)
  , m_PeakUsage(0)
  , m_CurrentUsage(0)
  , m_AllocationCount(0)
  , m_DeallocationCount(0) {}

void MemorySystem::Init() { LOG_INFO("<-- Initializing MemorySystem -->"); }

void MemorySystem::Shutdown() { LOG_INFO("<-- Shutting down MemorySystem -->"); }

void MemorySystem::ReportAllocation(
  void *ptr, const char *file, const char *func, usize size, int line, AllocationType type
) {
  if (!ptr) return;

  std::lock_guard<std::mutex> lock(m_Mutex);

  // Check for double allocation (potential bug)
  if (m_AllocTracker.find(ptr) != m_AllocTracker.end()) {
    LOG_WARN_F("Double allocation detected at %p in %s:%d", ptr, size, line);
  }

  m_AllocTracker[ptr] = AllocationInfo(file, func, size, line, type);
  m_TotalAllocated += size;
  m_CurrentUsage += size;
  m_AllocationCount++;

  if (m_CurrentUsage > m_PeakUsage) {
    m_PeakUsage = m_CurrentUsage;
  }
}

void MemorySystem::ReportDeallocation(void *ptr, const char *file, i32 line) {
  if (!ptr) return;

  std::lock_guard<std::mutex> lock(m_Mutex);

  auto it = m_AllocTracker.find(ptr);
  if (it == m_AllocTracker.end()) {
    LOG_WARN_F("Attempting to free untracked pointer %p in %s:%d", ptr, file, line);
    return;
  }

  m_TotalFreed += it->second.size;
  m_CurrentUsage -= it->second.size;
  m_DeallocationCount++;
  m_AllocTracker.erase(it);
}

std::string &&MemorySystem::GetLeaksReport() {
  std::lock_guard<std::mutex> lock(m_Mutex);

  static std::string buffer;
  std::stringstream oss;

  if (!m_AllocTracker.empty()) {
    oss << "\n=== Memory Leaks Detected ===" << std::endl;
    for (const auto &pair : m_AllocTracker) {
      const AllocationInfo &info = pair.second;
      oss
        << "Leak: "
        << pair.first
        << " ("
        << ToHumanReadable(info.size)
        << ") "
        << "allocated in "
        << (info.file ? info.file : "unknown")
        << ":"
        << info.line
        << " ("
        << (info.func ? info.func : "unknown")
        << ")"
        << std::endl;
    }
  } else {
    oss << "\n=== No Leaks Detected ===";
  }

  buffer = oss.str();
  return std::move(buffer);
}

std::string &&MemorySystem::GetAllocsReport() {
  std::lock_guard<std::mutex> lock(m_Mutex);

  static std::unordered_map<AllocationType, i32> allocTypeSums;
  allocTypeSums.clear();
  for (const auto &pair : m_AllocTracker) {
    const AllocationInfo &info = pair.second;
    if (allocTypeSums.find(info.type) != allocTypeSums.end()) {
      allocTypeSums[info.type] += info.size;
    } else {
      allocTypeSums[info.type] = info.size;
    }
  }

  static std::string buffer;
  std::stringstream oss;
  oss << "=== Memory Tracking Statistics ===" << std::endl;
  oss << "Total allocated: " << ToHumanReadable(m_TotalAllocated) << std::endl;
  oss << "Total freed: " << ToHumanReadable(m_TotalFreed) << std::endl;
  oss << "Current usage: " << ToHumanReadable(m_CurrentUsage) << std::endl;
  for (const auto &pair : allocTypeSums) {
    const i32 sum = pair.second;
    oss << "  " << kAllocationTypeStr[pair.first] << ": " << ToHumanReadable(sum) << std::endl;
  }
  oss << "Peak usage: " << ToHumanReadable(m_PeakUsage) << std::endl;
  oss << "Allocation count: " << m_AllocationCount << std::endl;
  oss << "Deallocation count: " << m_DeallocationCount << std::endl;
  oss << "Active allocations: " << m_AllocTracker.size();

  buffer = oss.str();
  return std::move(buffer);
}

std::string &&MemorySystem::ToHumanReadable(u64 byte) {
  static std::string buffer;
  buffer = (byte < SN_MEM_KIB) ? std::to_string(byte) + SN_MEM_B_STR :
    (byte < SN_MEM_MIB)        ? std::to_string(byte / SN_MEM_KIB) + SN_MEM_KIB_STR :
    (byte < SN_MEM_GIB)        ? std::to_string(byte / SN_MEM_MIB) + SN_MEM_MIB_STR :
                                 std::to_string(byte / SN_MEM_GIB) + SN_MEM_GIB_STR;
  return std::move(buffer);
}

void *SNAlloc(usize sizeBytes, const char *file, const char *func, i32 line, AllocationType type) {
#ifndef SN_NDEBUG
  SN_ASSERT(MemorySystem::GetPtr(), "Memory system is not initialized");
#endif // !SN_NDEBUG
  void *ptr = malloc(sizeBytes);
  MemorySystem::Get().ReportAllocation(ptr, file, func, sizeBytes, line, type);
  return ptr;
}

void SNFree(void *mem, const char *file, i32 line) {
#ifndef SN_NDEBUG
  SN_ASSERT(MemorySystem::GetPtr(), "Memory system is not initialized");
#endif // !SN_NDEBUG
  MemorySystem::Get().ReportDeallocation(mem, file, line);
  free(mem);
}

uintptr_t AlignAddress(uintptr_t ptr, usize align) {
  SN_ASSERT((align & (align - 1)) == 0, "alignment must be a power of two");
  SN_ASSERT(align > 0 && align <= 128, "alignment must be in range [1, 128]");

  uintptr_t a, mod;
  a = reinterpret_cast<uintptr_t>(align);
  mod = ptr & (a - 1);

  // ptr is aligned if mod == 0
  if (mod != 0) {
    ptr += a - mod;
  }

  return ptr;
}

uintptr_t AlignSize(uintptr_t size, usize align) { return (size + align - 1) & ~(align - 1); }

void *operator new(usize size, const char *file, const char *func, i32 line, AllocationType type) {
  return SNAlloc(size, file, func, line, type);
}

void operator delete(void *mem, const char *file, i32 line) { SNFree(mem, file, line); }

void operator delete[](void *mem, const char *file, i32 line) { SNFree(mem, file, line); }
