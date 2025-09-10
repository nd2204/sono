#ifndef SN_ALLOCATOR_H
#define SN_ALLOCATOR_H

#include <core/common/types.h>
#include <core/memory/allocation_info.h>
#include <utility>

class Allocator {
public:
  virtual void *Alloc(usize sizeInBytes = 0, AllocationType tag = ALLOC_TYPE_GENERAL) = 0;

  virtual void *AllocAlign(
    usize sizeInBytes = 0, u16 align = 16, AllocationType tag = ALLOC_TYPE_GENERAL
  ) {
    (void)align;
    return Alloc(sizeInBytes, tag);
  };

  virtual void Free(void *mem) = 0;

  virtual void FreeAlign(void *mem) { Free(mem); };

  template <typename T, AllocationType Tag = ALLOC_TYPE_GENERAL, typename... Args>
  T *New(Args &&...args) {
    void *mem = Alloc(sizeof(T), Tag);
    return new (mem) T(std::forward<Args>(args)...);
  }

  template <
    typename T, int Alignment = 16, AllocationType Tag = ALLOC_TYPE_GENERAL, typename... Args>
  T *NewAlign(Args &&...args) {
    void *mem = AllocAlign(sizeof(T), Alignment, Tag);
    return new (mem) T(std::forward<Args>(args)...);
  }
};

#endif // !SN_ALLOCATOR_H
