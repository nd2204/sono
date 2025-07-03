#ifndef SN_GL_BUFFER_H
#define SN_GL_BUFFER_H

#include "gl_buffer_base.h"
#include "render/vertex_buffer.h"

#include <cstdint>
#include <glad/glad.h>

class GLVertexBuffer
  : public IVertexBuffer
  , public GLBuffer {
public:
  /// @brief Constructs a Vertex Buffer Object (VBO) with the given data and size.
  /// @param usage (e.g., BufferUsage::STATIC, BufferUsage::DYNAMIC).
  /// @param vertSize Size of the vertex data in bytes.
  /// @param vertCount number of vertices
  GLVertexBuffer(BufferUsage usage, usize vertSize, usize vertCount);

  ~GLVertexBuffer() = default;

  virtual u32 GetStride() const override;

  virtual usize GetVertexSize() const override;

  virtual u32 GetVertexCount() const override;

private:
  u32 m_VertexSize;
  u32 m_NumVertices;
};

#endif // !SN_GL_BUFFER_H
