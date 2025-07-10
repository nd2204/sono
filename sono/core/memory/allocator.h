#ifndef SN_ALLOCATOR_H
#define SN_ALLOCATOR_H

#include "core/common/types.h"
#include <utility>

class Allocator {
public:
  virtual void *Alloc(usize sizeInBytes) = 0;
  virtual void *AllocAlign(usize sizeInBytes, u16 align) = 0;

  virtual void Free(void *mem) = 0;
  virtual void FreeAlign(void *mem) { (void)mem; };

  template <typename T, typename... Args>
  T *New(Args &&...args) {
    void *mem = Alloc(sizeof(T));
    return new (mem) T(std::forward<Args>(args)...);
  }

  template <typename T, int Alignment = 16, typename... Args>
  T *NewAlign(Args &&...args) {
    void *mem = AllocAlign(sizeof(T), Alignment);
    return new (mem) T(std::forward<Args>(args)...);
  }
};

#endif // !SN_ALLOCATOR_H
