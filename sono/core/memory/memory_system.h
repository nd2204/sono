#ifndef MEMORY_SYSTEM_H
#define MEMORY_SYSTEM_H

#include "core/system.h"
#include "core/common/types.h"
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

#if !defined(NDEBUG) && !defined(SN_NO_MEMTRACKING)
#define SN_ALLOC(size, type)                                                   \
  Sono::Memory::SNAlloc((size), (type), __FILE__, __LINE__)
#define SN_FREE(ptr) Sono::Memory::SNFree((ptr))
#else
#define SN_ALLOC(size, type) malloc(size)
#define SN_FREE(ptr)         free(ptr)
#endif

namespace Sono::Memory {

enum AllocationType {
  SN_ALLOCATION_TYPE_DEFAULT = 0,

  // Allocator
  SN_ALLOCATION_TYPE_ALLOCATOR_ARENA,
  SN_ALLOCATION_TYPE_ALLOCATOR_POOL,

  // Max value for using in arrays
  SN_ALLOCATION_TYPE_MAX
};

struct AllocationInfo {
  i32 line;
  std::string sourceFile;
  AllocationType type;
  usize size;
};

class MemorySystem : public SubSystem {
public:
  MemorySystem() = default;
  ~MemorySystem() override = default;

  /// @brief: Initialize the memory system
  void Init() override;

  /// @brief: Shutdown the memory system
  void Shutdown() override;

  /// @brief: Track memory allocation
  void TrackAllocation(
    void *ptr, size_t size, const char *file, int line, AllocationType type
  );

  /// @brief Track memory deallocation
  void TrackDeallocation(void *ptr);

  /// @brief: Generate a report of all tracked allocations
  void Report();

private:
  static std::unordered_map<void *, AllocationInfo> m_allocTracker;
};

std::string &&ToHumanReadable(u64 byte);

void *SNAlloc(
  usize sizeBytes, AllocationType type, std::string &&file, i32 line
);

void SNFree(void *mem);

uintptr_t AlignAddress(uintptr_t addr, usize align);

uintptr_t AlignSize(uintptr_t size, usize align);

} // namespace Sono::Memory

#endif // !MEMORY_SYSTEM_H