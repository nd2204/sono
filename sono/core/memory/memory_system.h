#ifndef SN_MEMORY_SYSTEM_H
#define SN_MEMORY_SYSTEM_H

#include "allocator.h"
#include "allocators/heap.h"
#include "core/common/types.h"
#include "core/common/defines.h"
#include "core/common/singleton.h"
#include <mutex>
#include <string>
#include <unordered_map>

constexpr usize SN_MEM_KIB = 1024;
constexpr usize SN_MEM_MIB = SN_MEM_KIB * SN_MEM_KIB;
constexpr usize SN_MEM_GIB = SN_MEM_MIB * SN_MEM_MIB;

constexpr usize SN_MEM_KB = 1000;
constexpr usize SN_MEM_MB = SN_MEM_KIB * SN_MEM_KIB;
constexpr usize SN_MEM_GB = SN_MEM_MIB * SN_MEM_MIB;

#define SN_MEM_B_STR  "B"
#define SN_MEM_KB_STR "KB"
#define SN_MEM_MB_STR "MB"
#define SN_MEM_GB_STR "GB"

#define SN_MEM_KIB_STR "KiB"
#define SN_MEM_MIB_STR "MiB"
#define SN_MEM_GIB_STR "GiB"

#define GIB(byte) (byte / SN_MEM_GIB)
#define MIB(byte) (byte / SN_MEM_MIB)
#define KIB(byte) (byte / SN_MEM_KIB)

// clang-format off

#define __FOREACH_ALLOCATION_TYPES(F) \
  F(0, ALLOC_TYPE_GENERAL),           \
  F(1, ALLOC_TYPE_SYSTEM_EVENT),      \
  F(2, ALLOC_TYPE_ALLOCATOR_ARENA),   \
  F(3, ALLOC_TYPE_ALLOCATOR_POOL),    \
  F(4, ALLOC_TYPE_RESOURCE),          \
  F(5, ALLOC_TYPE_RENDER_SYSTEM),     \
  F(6, ALLOC_TYPE_MAX)

// clang-format on

DEFINE_ENUMS(AllocationType, __FOREACH_ALLOCATION_TYPES);

struct AllocationInfo {
  const char *file;
  const char *func;
  usize size;
  i32 line;
  AllocationType type;

  AllocationInfo()
    : file(nullptr)
    , func(nullptr)
    , size(0)
    , line(0)
    , type(ALLOC_TYPE_GENERAL) {}

  AllocationInfo(const char *file, const char *func, usize size, i32 line, AllocationType type)
    : file(file)
    , func(func)
    , size(size)
    , line(line)
    , type(type) {}
};

class MemorySystem : public Singleton<MemorySystem> {
public:
  MemorySystem();

  ~MemorySystem();

  MemorySystem(MemorySystem &rhs) = delete;

  MemorySystem &operator=(MemorySystem &rhs) = delete;

  void Init();

  void Shutdown();

  /// @brief: update memory allocation
  void ReportAllocation(
    void *ptr, const char *file, const char *func, usize size, int line, AllocationType type
  );

  /// @brief update memory deallocation
  void ReportDeallocation(void *ptr, const char *file, i32 line);

  /// @brief: Generate a report of all tracked allocations
  std::string GetAllocsReport() const;

  /// @brief: Generate a report of memory leaks
  std::string GetLeaksReport() const;

  Allocator &GetGlobalAllocator();

public:
  static std::string ToHumanReadable(u64 byte);
  static std::string ToHumanReadableValueStr(u64 byte);

private:
  HeapAllocator m_GlobalAllocator;

  std::unordered_map<void *, AllocationInfo> m_AllocTracker;
  std::mutex m_Mutex;
  usize m_TotalAllocated;
  usize m_TotalFreed;
  usize m_PeakUsage;
  usize m_CurrentUsage;
  u32 m_AllocationCount;
  u32 m_DeallocationCount;
};

void SNZero(void *mem, usize sizeInBytes);

void *SNAlloc(usize sizeBytes, const char *file, const char *func, i32 line, AllocationType type);

void SNFree(void *mem, const char *file, i32 line);

void *operator new(usize size, const char *file, const char *func, i32 line, AllocationType type);

void operator delete(void *mem, const char *file, i32 line);

void operator delete[](void *mem, const char *file, i32 line);

uintptr_t AlignAddress(uintptr_t addr, usize align);

uintptr_t AlignSize(uintptr_t size, usize align);

#define SN_ZERO(ptr, size) memset((ptr), 0, (size));

#if !defined(SN_NDEBUG) && !defined(SN_NO_MEMTRACKING)
#define SN_ALLOC(size, type) SNAlloc((size), __FILE__, __FUNCTION__, __LINE__, (type))
#define SN_FREE(ptr)         SNFree(ptr, __FILE__, __LINE__)
#define SN_NEW(type)         new (__FILE__, __FUNCTION__, __LINE__, (type))
#define SN_DELETE            delete (__FILE__, __LINE__)
#else
#define SN_ALLOC(size, type) malloc(size)
#define SN_FREE(ptr)         free(ptr)
#define SN_NEW(type)         new
#define SN_DELETE            delete
#endif

#endif // !SN_MEMORY_SYSTEM_H
