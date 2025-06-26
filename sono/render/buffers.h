#ifndef SN_BUFFER_H
#define SN_BUFFER_H

#include "core/common/types.h"

enum BufferType : u8 { BUF_TYPE_INDEX, BUF_TYPE_VERTEX, BUF_TYPE_UNIFORM };

class IBuffer {
public:
  virtual ~IBuffer() = default;
  virtual BufferType GetTypes() = 0;
  virtual u32 GetSize() = 0;
};

class IndexBuffer : public IBuffer {
  BufferType GetTypes() override { return BufferType::BUF_TYPE_INDEX; };
  u32 GetSize() override;
};

class VertexBuffer : public IBuffer {
  BufferType GetTypes() override { return BufferType::BUF_TYPE_VERTEX; };
  u32 GetSize() override;
};

#endif // !SN_BUFFER_H
