#ifndef SN_HEAP_ALLOCATOR_H
#define SN_HEAP_ALLOCATOR_H

#include "core/memory/allocator.h"

class HeapAllocator : public Allocator {
public:
  void *Alloc(usize sizeInBytes, AllocationType tag) override;

  void *AllocAlign(usize sizeInBytes, u16 align, AllocationType tag) override;

  void Free(void *mem) override;

  void FreeAlign(void *mem) override;
};

#endif // !SN_HEAP_ALLOCATOR_H
