#include "heap.h"
#include "core/memory/memory_system.h"

void *HeapAllocator::Alloc(usize sizeInBytes, AllocationType tag) {
  return SN_ALLOC(sizeInBytes, tag);
}
// --------------------------------------------------------------------------------
void *HeapAllocator::AllocAlign(usize sizeInBytes, u16 align, AllocationType tag) {
  SN_ASSERT(false, "HeapAllocator::AllocAlign Not implemented");
  usize alignedSize = AlignSize(sizeInBytes, align);
  return SN_ALLOC(alignedSize, tag);
}
// --------------------------------------------------------------------------------
void HeapAllocator::Free(void *mem) { SN_FREE(mem); }
// --------------------------------------------------------------------------------
void HeapAllocator::FreeAlign(void *mem) { SN_FREE(mem); }
