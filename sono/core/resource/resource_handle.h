#ifndef SN_RESOURCE_HANDLE_H
#define SN_RESOURCE_HANDLE_H

#include "core/common/snassert.h"
#include "core/common/types.h"

class Handle {
private:
  // Both combined must be smaller than 32
  static constexpr u32 MAX_INDEX_BITS = 16;
  static constexpr u32 MAX_MAGIC_BITS = 16;

  static constexpr u32 MAX_INDEX = U16_MAX;
  static constexpr u32 MAX_MAGIC = U16_MAX;

public:
  using HandleType = u32;

  static inline u32 CreateFromIndex(u32 index) {
    ASSERT(index < MAX_INDEX);

    static u16 s_AutoMagic = 0;
    if (++s_AutoMagic > MAX_MAGIC) {
      s_AutoMagic = 1;
    }

    HandleType handle{0};
    handle |= index << MAX_INDEX_BITS;
    handle += s_AutoMagic;
    return handle;
  }

  static inline constexpr u32 GetIndex(HandleType h) { return h & MAX_INDEX; }

  static inline constexpr u32 GetMagic(HandleType h) { return (h & MAX_MAGIC) >> MAX_MAGIC_BITS; }

  static inline constexpr bool IsValid(HandleType h) { return h != 0; }
};

#endif // !SN_RESOURCE_HANDLE_H
