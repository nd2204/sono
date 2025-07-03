#include "core/common/snassert.h"

#include "gl_index_buffer.h"
#include "gl_buffer_base.h"
#include "gl_buffer_manager.h"

// ------------------------------------------------------------------------------------------
GLIndexBuffer::GLIndexBuffer(BufferUsage usage, IndexType type, usize idxCount)
  : GLBuffer(GL_ELEMENT_ARRAY_BUFFER, usage, IndexTypeSize(type) * idxCount)
  , m_Count(idxCount)
  , m_IndexType(type) {}
// ------------------------------------------------------------------------------------------
IndexType GLIndexBuffer::GetIndexType() const { return m_IndexType; };
// ------------------------------------------------------------------------------------------
u32 GLIndexBuffer::GetIndexCount() const { return m_Count; };

// clang-format off

GLenum GLIndexBuffer::ConvertIndexType(IndexType type) {
  switch (type) {
    case INDEX_TYPE_U16: return GL_UNSIGNED_SHORT;
    case INDEX_TYPE_U32: return GL_UNSIGNED_INT;
  }
  ASSERT(false);
  return 0;
}

GLuint GLIndexBuffer::IndexTypeSize(IndexType type) {
  switch (type) {
    case INDEX_TYPE_U16: return sizeof(u16);
    case INDEX_TYPE_U32: return sizeof(u32);
  }
  ASSERT(false);
  return 0;
}

// clang-format on
