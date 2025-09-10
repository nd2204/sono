#include "memory_system.h"
#include "../common/snassert.h"
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <unordered_map>

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

DEFINE_STRING_CONSTANTS(kAllocationTypeStr, __FOREACH_ALLOCATION_TYPES);

// --------------------------------------------------------------------------------
MemorySystem::MemorySystem()
  : m_TotalAllocated(0)
  , m_TotalFreed(0)
  , m_PeakUsage(0)
  , m_CurrentUsage(0)
  , m_AllocationCount(0)
  , m_DeallocationCount(0) {}
// --------------------------------------------------------------------------------
MemorySystem::~MemorySystem() {}
// --------------------------------------------------------------------------------
void MemorySystem::Init() {
  System::Init();
  LOG_INFO("<-- Initializing MemorySystem -->");
}
// --------------------------------------------------------------------------------
void MemorySystem::Shutdown() {
  System::Shutdown();
  LOG_MSG(MemorySystem::GetPtr()->GetAllocsReport().c_str());
  LOG_MSG(MemorySystem::GetPtr()->GetLeaksReport().c_str());
}
// --------------------------------------------------------------------------------
Allocator &MemorySystem::GetGlobalAllocator() { return m_GlobalAllocator; }
// --------------------------------------------------------------------------------
void MemorySystem::ReportSubAllocation(
  void *parentPtr, void *childPtr, const char *file, const char *func, usize size, int line,
  AllocationType type
) {
#ifndef SN_NO_MEMTRACKING
  if (!parentPtr || !childPtr || m_AllocTracker.find(parentPtr) == m_AllocTracker.end()) return;

  std::lock_guard<std::mutex> lock(m_Mutex);

  auto &childMap = m_AllocTracker[parentPtr].childs;
  if (childMap.find(childPtr) != childMap.end()) {
    LOG_WARN_F("Double sub-allocation detected at %p in %s:%d", childPtr, size, line);
  }

  childMap[childPtr] = {
    .file = file,
    .func = func,
    .parent = &m_AllocTracker[parentPtr],
    .size = size,
    .line = line,
    .type = type
  };
#endif // !SN_NO_MEMTRACKING
}
// --------------------------------------------------------------------------------
// --------------------------------------------------------------------------------
void MemorySystem::ReportAllocation(
  void *ptr, const char *file, const char *func, usize size, int line, AllocationType type
) {
#ifndef SN_NO_MEMTRACKING
  if (!ptr) return;

  std::lock_guard<std::mutex> lock(m_Mutex);

  // Check for double allocation (potential bug)
  if (m_AllocTracker.find(ptr) != m_AllocTracker.end()) {
    LOG_WARN_F("Double allocation detected at %p in %s:%d", ptr, size, line);
  }

  // clang-format off
  m_AllocTracker[ptr] = {
    .file = file,
    .func = func,
    .parent = nullptr,
    .size = size,
    .line = line,
    .type = type
  };

  // clang-format on
  m_TotalAllocated += size;
  m_CurrentUsage += size;
  m_AllocationCount++;

  if (m_CurrentUsage > m_PeakUsage) {
    m_PeakUsage = m_CurrentUsage;
  }
#endif // !SN_NO_MEMTRACKING
}
// --------------------------------------------------------------------------------
void MemorySystem::ReportDeallocation(void *ptr, const char *file, i32 line) {
#ifndef SN_NO_MEMTRACKING
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
#endif // !SN_NO_MEMTRACKING
}
// --------------------------------------------------------------------------------
std::string MemorySystem::GetLeaksReport() const {
#ifndef SN_NO_MEMTRACKING
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

  return oss.str();
#else
  return "";
#endif // !SN_NO_MEMTRACKING
}
// --------------------------------------------------------------------------------
std::string MemorySystem::GetAllocsReport() const {
  std::unordered_map<AllocationType, i32> allocTypeSums;
  allocTypeSums.clear();

  // TODO: add recursive report string collector for allocation with childs
  for (const auto &pair : m_AllocTracker) {
    const AllocationInfo &info = pair.second;
    if (allocTypeSums.find(info.type) != allocTypeSums.end()) {
      allocTypeSums[info.type] += info.size;
    } else {
      allocTypeSums[info.type] = info.size;
    }
  }

  std::stringstream oss;
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

  return oss.str();
}
// --------------------------------------------------------------------------------
std::string MemorySystem::ToHumanReadable(u64 byte) {
  std::string buffer = ToHumanReadableValueStr(byte);
  // clang-format off
  buffer += (byte < SN_MEM_KIB) ? SN_MEM_B_STR :
            (byte < SN_MEM_MIB) ? SN_MEM_KIB_STR :
            (byte < SN_MEM_GIB) ? SN_MEM_MIB_STR :
                                  SN_MEM_GIB_STR;
  // clang-format on
  return buffer;
}
// --------------------------------------------------------------------------------
std::string MemorySystem::ToHumanReadableValueStr(u64 byte) {
  if (byte < SN_MEM_KIB) return std::to_string(byte);
  if (byte < SN_MEM_MIB) return std::to_string(byte / SN_MEM_KIB);
  if (byte < SN_MEM_GIB) return std::to_string(byte / SN_MEM_MIB);
  return std::to_string(byte / SN_MEM_GIB);
}
// --------------------------------------------------------------------------------
void *SNAlloc(usize sizeBytes, const char *file, const char *func, i32 line, AllocationType type) {
  void *ptr = malloc(sizeBytes);
#ifndef SN_NO_MEMTRACKING
  MemorySystem *pMemSys = MemorySystem::GetPtr();
  if (!pMemSys) {
    LOG_WARN("'Untracked Allocation' - Using SNAlloc before memory system initialization");
  } else {
    pMemSys->ReportAllocation(ptr, file, func, sizeBytes, line, type);
  }
#endif // !SN_NO_MEMTRACKING
  return ptr;
}
// --------------------------------------------------------------------------------
void SNFree(void *mem, const char *file, i32 line) {
#ifndef SN_NO_MEMTRACKING
  SN_ASSERT(MemorySystem::GetPtr(), "Memory system is not initialized");
  MemorySystem::Get().ReportDeallocation(mem, file, line);
#endif // !SN_NO_MEMTRACKING
  free(mem);
}
// --------------------------------------------------------------------------------
AlignResult AlignForward(uintptr_t ptr, usize align) {
  SN_ASSERT((align & (align - 1)) == 0, "alignment must be a power of two");
  SN_ASSERT(align > 0 && align <= 128, "alignment must be in range [1, 128]");

  uintptr_t a, mod;
  a = reinterpret_cast<uintptr_t>(align);
  mod = ptr & (a - 1);

  usize offset = a - mod;
  // ptr is aligned if mod == 0
  if (mod != 0) {
    ptr += offset;
  }

  return {ptr, offset};
}
// --------------------------------------------------------------------------------
void *operator new(usize size, const char *file, const char *func, i32 line, AllocationType type) {
  return SNAlloc(size, file, func, line, type);
}
// --------------------------------------------------------------------------------
void operator delete(void *mem, const char *file, i32 line) { SNFree(mem, file, line); }
// --------------------------------------------------------------------------------
void operator delete[](void *mem, const char *file, i32 line) { SNFree(mem, file, line); }
