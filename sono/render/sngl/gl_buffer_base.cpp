#include "core/common/snassert.h"

#include "gl_buffer_manager.h"
#include "gl_buffer_base.h"

// ------------------------------------------------------------------------------------------
GLBuffer::GLBuffer(GLenum target, BufferUsage usage, usize size)
  : m_Target(target)
  , m_Size(size)
  , m_Usage(usage) {
  glGenBuffers(1, &m_BufferID);
}
// ------------------------------------------------------------------------------------------
void *GLBuffer::Map() {
  glBindBuffer(m_Target, m_BufferID);
  return glMapBuffer(m_Target, GL_WRITE_ONLY);
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Unmap() {
  glBindBuffer(m_Target, m_BufferID);
  glUnmapBuffer(m_Target);
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Update(const void *data, usize size, usize offset) {
  ASSERT(offset + size <= m_Size);
  glBindBuffer(m_Target, m_BufferID);
  if (offset == 0 && size == m_Size) {
    glBufferData(m_Target, size, data, GLBufferManager::ConvertBufferUsage(m_Usage));
  } else {
    glBufferSubData(m_Target, offset, size, data);
  }
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Release() { glDeleteBuffers(1, &m_BufferID); }
// ------------------------------------------------------------------------------------------
u32 GLBuffer::GetSize() const { return m_Size; };
// ------------------------------------------------------------------------------------------
BufferUsage GLBuffer::GetUsage() const { return m_Usage; };
// ------------------------------------------------------------------------------------------
void GLBuffer::Bind() const { glBindBuffer(m_Target, m_BufferID); }
// ------------------------------------------------------------------------------------------
void GLBuffer::Unbind() const { glBindBuffer(m_Target, 0); }
// ------------------------------------------------------------------------------------------
GLuint GLBuffer::GetID() const { return m_BufferID; }
// ------------------------------------------------------------------------------------------
GLBuffer::operator u32() const { return m_BufferID; }
// ------------------------------------------------------------------------------------------
GLBuffer::~GLBuffer() { glDeleteBuffers(1, &m_BufferID); }
