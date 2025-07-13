#ifndef GL_INDEX_BUFFER_H
#define GL_INDEX_BUFFER_H

#include "core/common/types.h"
#include "render/buffer/index_buffer.h"
#include "gl_buffer_base.h"

#include "glad/glad.h"

class GLIndexBuffer
  : public IIndexBuffer
  , public GLBuffer {
public:
  GLIndexBuffer(BufferUsage usage, IndexType type, usize idxCount);

  ~GLIndexBuffer() = default;

  virtual IndexType GetIndexType() const override;

  virtual u32 GetIndexCount() const override;

public:
  static GLenum ConvertIndexType(IndexType type);
  static GLuint IndexTypeSize(IndexType type);

private:
  u32 m_Count;
  IndexType m_IndexType;
};

#endif // !GL_INDEX_BUFFER_H
