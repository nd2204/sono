#include "core/common/snassert.h"

#include "gl_vertex_buffer.h"
#include "gl_buffer_base.h"
#include "gl_buffer_manager.h"

// ------------------------------------------------------------------------------------------
GLVertexBuffer::GLVertexBuffer(BufferUsage usage, usize vertSize, usize vertCount)
  : GLBuffer(GL_ARRAY_BUFFER, usage, vertSize * vertCount)
  , m_VertexSize(vertSize)
  , m_NumVertices(vertCount) {}
// ------------------------------------------------------------------------------------------
u32 GLVertexBuffer::GetStride() const { return m_NumVertices * m_VertexSize; };
// ------------------------------------------------------------------------------------------
usize GLVertexBuffer::GetVertexSize() const { return m_VertexSize; };
// ------------------------------------------------------------------------------------------
u32 GLVertexBuffer::GetVertexCount() const { return m_NumVertices; };
