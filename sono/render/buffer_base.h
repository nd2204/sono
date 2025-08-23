#ifndef SN_BUFFER_H
#define SN_BUFFER_H

#include "core/common/defines.h"
#include "core/common/types.h"

enum class BufferUsage : u32 {
  // clang-format off
  None     = 0,
  Vertex   = BITVAL(1),
  Index    = BITVAL(2),
  Uniform  = BITVAL(3),
  Storage  = BITVAL(4),
  Indirect = BITVAL(5),
  CopySrc  = BITVAL(6),
  CopyDest = BITVAL(7),
  MapRead  = BITVAL(8),
  MapWrite = BITVAL(9),
  // clang-format on
};

inline BufferUsage operator|(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<BufferUsage>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
}

inline u32 operator&(BufferUsage lhs, BufferUsage rhs) {
  return static_cast<u32>(lhs) & static_cast<u32>(rhs);
}

struct BufferDesc {
  usize count;
  u32 stride;
  BufferUsage usage = BufferUsage::None;
  const void *data = nullptr;
};

class Buffer {
public:
  Buffer(const BufferDesc &desc)
    : m_Desc(desc) {}

  virtual ~Buffer() = default;

  /// Bind the buffer
  virtual void Bind() const = 0;

  /// Unbind the buffer
  virtual void Unbind() const = 0;

  /// Map this buffer to cpu
  virtual void *Map() = 0;

  /// Unmap this buffer from the cpu
  virtual void Unmap() = 0;

  /// Write the data to the buffer
  virtual void Update(const void *data, usize size, usize offset = 0) = 0;

  /// Release the resources from the gpu
  virtual void Release() = 0;

  /// Get the buffer usage
  inline BufferUsage GetUsage() const { return m_Desc.usage; }

  /// Get the size in bytes of the data
  inline u32 GetCount() const { return m_Desc.count; }

  /// Get the size in bytes of the data
  inline u32 GetSize() const { return m_Desc.count * m_Desc.stride; }

  /// Get the size in bytes of the data
  inline u32 GetStride() const { return m_Desc.stride; }

protected:
  BufferDesc m_Desc;
};

#endif // !SN_BUFFER_H
