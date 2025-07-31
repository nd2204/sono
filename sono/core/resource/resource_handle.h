#ifndef SN_RESOURCE_HANDLE_H
#define SN_RESOURCE_HANDLE_H

#include "core/common/snassert.h"
#include "core/common/types.h"

template <typename TAG>
class ResourceHandle {
public:
  ResourceHandle()
    : m_Handle{0} {}

  inline void Init(u32 index) {
    ASSERT(IsNull());
    ASSERT(index < MAX_INDEX);

    static u16 s_AutoMagic = 0;
    if (++s_AutoMagic > MAX_MAGIC) {
      s_AutoMagic = 1;
    }

    m_Field.index = index;
    m_Field.magic = s_AutoMagic;
  }

  inline u32 GetIndex() const { return m_Field.index; }

  inline u32 GetMagic() const { return m_Field.magic; }

  inline u32 GetHandle() const { return m_Handle; }

  inline bool IsNull() const { return !m_Handle; }

  friend inline bool operator==(const ResourceHandle<TAG> &lhs, const ResourceHandle<TAG> &rhs) {
    return lhs.GetHandle() == rhs.GetHandle();
  }

  friend inline bool operator!=(const ResourceHandle<TAG> &lhs, const ResourceHandle<TAG> &rhs) {
    return lhs.GetHandle() != rhs.GetHandle();
  }

  operator u32() { return !m_Handle; }

private:
  // Both combined must be smaller than 32
  static constexpr u32 MAX_INDEX_BITS = 16;
  static constexpr u32 MAX_MAGIC_BITS = 16;

  static constexpr u32 MAX_INDEX = U16_MAX;
  static constexpr u32 MAX_MAGIC = U16_MAX;

  struct Fields {
    u32 index : MAX_INDEX_BITS;
    u32 magic : MAX_MAGIC_BITS;
  };

  union {
    Fields m_Field;
    u32 m_Handle;
  };
};

#endif // !SN_RESOURCE_HANDLE_H
