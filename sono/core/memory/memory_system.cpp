#include "memory_system.h"
#include "../common/snassert.h"
#include <cstdint>
#include <cstdlib>

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

using namespace Sono::Memory;

void MemorySystem::Init() {}

void MemorySystem::TrackAllocation(
  void *ptr, size_t size, const char *file, int line, AllocationType type
) {}

void MemorySystem::TrackDeallocation(void *ptr) {}

void MemorySystem::Report() {}

void MemorySystem::Shutdown() {}

static inline bool is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

static MemorySystem g_memorySystem;

std::string &&ToHumanReadable(u64 byte) {
  static std::string buffer;
  buffer = (byte < SN_MEM_KIB) ? std::to_string(byte) + SN_MEM_B_STR :
    (byte < SN_MEM_MIB) ? std::to_string(byte / SN_MEM_KIB) + SN_MEM_KIB_STR :
    (byte < SN_MEM_GIB) ? std::to_string(byte / SN_MEM_MIB) + SN_MEM_MIB_STR :
                          std::to_string(byte / SN_MEM_GIB) + SN_MEM_GIB_STR;
  return std::move(buffer);
}

void *SNAlloc(
  usize sizeBytes, AllocationType type, std::string &&file, i32 line
) {
  void *ptr = malloc(sizeBytes);
  g_memorySystem.TrackAllocation(ptr, sizeBytes, file.c_str(), line, type);
  return ptr;
}

void SNFree(void *mem) {
  g_memorySystem.TrackDeallocation(mem);
  free(mem);
}

uintptr_t AlignAddress(uintptr_t ptr, usize align) {
  SN_ASSERT(is_power_of_two(align), "alignment is not a power of two");
  SN_ASSERT(align > 0, "alignment must be > 0");

  uintptr_t a, mod;
  a = reinterpret_cast<uintptr_t>(align);
  mod = ptr & (a - 1);

  // ptr is aligned if mod == 0
  if (mod != 0) {
    ptr += a - mod;
  }

  return ptr;
}

uintptr_t AlignSize(uintptr_t size, usize align) {
  return (size + align - 1) & ~(align - 1);
}
