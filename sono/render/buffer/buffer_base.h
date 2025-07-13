#ifndef SN_BUFFER_H
#define SN_BUFFER_H

#include "core/common/types.h"

enum class BufferUsage : u8 { STATIC, DYNAMIC, STREAM };

class IBuffer {
public:
  virtual ~IBuffer() = default;

  /// Bind the buffer
  virtual void Bind() const = 0;

  /// Unbind the buffer
  virtual void Unbind() const = 0;

  /// Map this buffer to cpu
  virtual void *Map() = 0;

  /// Unmap this buffer from the cpu
  virtual void Unmap() = 0;

  /// Get the size in bytes of the data
  virtual u32 GetSize() const = 0;

  /// Write the data to the buffer
  virtual void Update(const void *data, usize size, usize offset = 0) = 0;

  /// Get the buffer usage
  virtual BufferUsage GetUsage() const = 0;

  /// Release the resources from the gpu
  virtual void Release() = 0;
};

#endif // !SN_BUFFER_H
