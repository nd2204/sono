#ifndef MEMORY_SYSTEM_H
#define MEMORY_SYSTEM_H

#include "core/common/types.h"
#include "core/common/singleton.h"
#include <mutex>
#include <string>
#include <unordered_map>

#define SN_MEM_KIB 1024
#define SN_MEM_MIB (SN_MEM_KIB * SN_MEM_KIB)
#define SN_MEM_GIB (SN_MEM_KIB * SN_MEM_MIB)

#define SN_MEM_KB 1000
#define SN_MEM_MB (SN_MEM_KB * SN_MEM_KB)
#define SN_MEM_GB (SN_MEM_KB * SN_MEM_MB)

#define SN_MEM_B_STR  "B"
#define SN_MEM_KB_STR "KB"
#define SN_MEM_MB_STR "MB"
#define SN_MEM_GB_STR "GB"

#define SN_MEM_KIB_STR "KiB"
#define SN_MEM_MIB_STR "MiB"
#define SN_MEM_GIB_STR "GiB"

#define GIBIBYTES(byte) (byte / SN_MEM_GIB)
#define MEBIBYTES(byte) (byte / SN_MEM_MIB)
#define KIBIBYTES(byte) (byte / SN_MEM_KIB)

enum AllocationType {
  SN_ALLOCATION_TYPE_DEFAULT = 0,

  // Allocator
  SN_ALLOCATION_TYPE_ALLOCATOR_ARENA = 1,
  SN_ALLOCATION_TYPE_ALLOCATOR_POOL,

  // Resource
  SN_ALLOCATION_TYPE_RESOURCE,

  // Render

  // Max value for using in arrays
  SN_ALLOCATION_TYPE_MAX
};

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
    , type(SN_ALLOCATION_TYPE_DEFAULT) {}

  AllocationInfo(
    const char *file,
    const char *func,
    usize size,
    i32 line,
    AllocationType type
  )
    : file(file)
    , func(func)
    , size(size)
    , line(line)
    , type(type) {}
};

class MemorySystem : public Singleton<MemorySystem> {
public:
  MemorySystem();
  ~MemorySystem() = default;
  MemorySystem(MemorySystem &rhs) = delete;
  MemorySystem &operator=(MemorySystem &rhs) = delete;

  void Init();
  void Shutdown();

  /// @brief: update memory allocation
  void ReportAllocation(
    void *ptr,
    const char *file,
    const char *func,
    usize size,
    int line,
    AllocationType type
  );

  /// @brief update memory deallocation
  void ReportDeallocation(void *ptr, const char *file, i32 line);

  /// @brief: Generate a report of all tracked allocations
  std::string &&GetMemoryReport();

  /// @brief: Prints a report of all tracked allocations
  void PrintMemoryReport();

private:
  std::string &&ToHumanReadable(u64 byte);
  std::unordered_map<void *, AllocationInfo> m_AllocTracker;
  std::mutex m_Mutex;
  usize m_TotalAllocated;
  usize m_TotalFreed;
  usize m_PeakUsage;
  usize m_CurrentUsage;
  u32 m_AllocationCount;
  u32 m_DeallocationCount;
};

void *SNAlloc(
  usize sizeBytes,
  const char *file,
  const char *func,
  i32 line,
  AllocationType type
);

void SNFree(void *mem, const char *file, i32 line);

void *operator new(usize size);
void *operator new(usize size);

#if !defined(SN_NDEBUG) && !defined(SN_NO_MEMTRACKING)
#define SN_ALLOC(size, type)                                                   \
  SNAlloc((size), __FILE__, __FUNCTION__, __LINE__, (type))
#define SN_FREE(ptr) SNFree(ptr, __FILE__, __LINE__)
#define SN_NEW       new
#define SN_DELETE    delete
#else
#define SN_ALLOC(size, type) malloc(size)
#define SN_FREE(ptr)         free(ptr)
#define SN_NEW               new
#define SN_DELETE            delete
#endif

uintptr_t AlignAddress(uintptr_t addr, usize align);

uintptr_t AlignSize(uintptr_t size, usize align);

#endif // !MEMORY_SYSTEM_H