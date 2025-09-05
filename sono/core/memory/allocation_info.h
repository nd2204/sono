#ifndef SN_ALLOCATION_INFO_H
#define SN_ALLOCATION_INFO_H

#include <core/common/types.h>
#include <core/common/defines.h>
#include <map>

// clang-format off
#define __FOREACH_ALLOCATION_TYPES(F)                                                              \
  F(0, ALLOC_TYPE_GENERAL),         \
  F(1, ALLOC_TYPE_SYSTEM_EVENT),    \
  F(2, ALLOC_TYPE_ALLOCATOR_ARENA), \
  F(3, ALLOC_TYPE_ALLOCATOR_POOL),  \
  F(4, ALLOC_TYPE_RESOURCE),        \
  F(5, ALLOC_TYPE_RENDER_SYSTEM),   \
  F(6, ALLOC_TYPE_MAX)
// clang-format on

DEFINE_ENUMS(AllocationType, __FOREACH_ALLOCATION_TYPES);

struct AllocationInfo {
  const char *file = nullptr;
  const char *func = nullptr;
  std::map<void *, AllocationInfo> childs = {};
  AllocationInfo *parent = nullptr;
  usize size = 0;
  i32 line = 0;
  AllocationType type = ALLOC_TYPE_GENERAL;
};

#endif // !SN_ALLOCATION_INFO_H
