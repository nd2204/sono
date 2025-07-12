#ifndef SN_HEAP_ALLOCATOR_H
#define SN_HEAP_ALLOCATOR_H

#include "core/memory/allocator.h"

class HeapAllocator : public Allocator {
public:
  void *Alloc(usize sizeInBytes) override;

  void *AllocAlign(usize sizeInBytes, u16 align) override;

  void Free(void *mem) override;
};

#endif // !SN_HEAP_ALLOCATOR_H
