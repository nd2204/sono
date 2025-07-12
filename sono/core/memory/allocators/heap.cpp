#include "heap.h"
#include "core/memory/memory_system.h"

void *HeapAllocator::Alloc(usize sizeInBytes) { return SN_ALLOC(sizeInBytes, ALLOC_TYPE_GENERAL); }

void *HeapAllocator::AllocAlign(usize sizeInBytes, u16 align) {
  SN_ASSERT(false, "HeapAllocator::AllocAlign Not implemented");
  return nullptr;
}

void HeapAllocator::Free(void *mem) { SN_FREE(mem); }
