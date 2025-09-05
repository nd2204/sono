#include "core/common/snassert.h"

#include <render-backend/sngl/gl_render_device.h>
#include <render-backend/sngl/gl_buffer_base.h>
#include <render-backend/sngl/gl_commmon.h>
#include <render/buffer_base.h>

// ------------------------------------------------------------------------------------------
GLBuffer::GLBuffer(const BufferDesc &desc)
  : Buffer(desc) {

  m_Target = GL_ARRAY_BUFFER;
  if (desc.usage & BufferUsage::Index) m_Target = GL_ELEMENT_ARRAY_BUFFER;
  if (desc.usage & BufferUsage::Uniform) m_Target = GL_UNIFORM_BUFFER;
  // TODO add preprocess condition for opengl version
  // Only available in opengl 4+
  // if (desc.usage & BufferUsage::Storage) m_Target = GL_SHADER_STORAGE_BUFFER;
  // if (desc.usage & BufferUsage::Indirect) m_Target = GL_SHADER_STORAGE_BUFFER;

  m_GlUsage = (desc.usage & BufferUsage::MapWrite) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

  glGenBuffers(1, &m_BufferID);
  glBindBuffer(m_Target, m_BufferID);
  glBufferData(m_Target, GetSize(), desc.data, m_GlUsage);
  GL_CHECK_ERROR();
}
// ------------------------------------------------------------------------------------------
void *GLBuffer::Map() {
  glBindBuffer(m_Target, m_BufferID);
  void *ptr = nullptr;
  SN_ASSERT(
    m_Desc.usage & BufferUsage::MapWrite || m_Desc.usage & BufferUsage::MapRead,
    "Attemping to map an non mappable buffer. "
    "Enable BufferUsage::MapRead or BufferUsage::MapRead for mapping"
  );
  if (m_Desc.usage & BufferUsage::MapWrite && m_Desc.usage & BufferUsage::MapRead) {
    ptr = glMapBuffer(m_Target, GL_READ_WRITE);
  } else if (m_Desc.usage & BufferUsage::MapWrite) {
    ptr = glMapBuffer(m_Target, GL_WRITE_ONLY);
  } else {
    ptr = glMapBuffer(m_Target, GL_READ_ONLY);
  }
  GL_CHECK_ERROR();
  return ptr;
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Unmap() {
  glBindBuffer(m_Target, m_BufferID);
  glUnmapBuffer(m_Target);
  GL_CHECK_ERROR();
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Update(const void *data, usize size, usize offset) {
  ASSERT(offset + size <= GetSize());
  glBindBuffer(m_Target, m_BufferID);
  GL_CHECK_ERROR();
  if (offset == 0 && size == GetSize()) {
    glBufferData(m_Target, size, data, m_GlUsage);
    GL_CHECK_ERROR();
  } else {
    glBufferSubData(m_Target, offset, size, data);
    GL_CHECK_ERROR();
  }
}
// ------------------------------------------------------------------------------------------
void GLBuffer::Release() { glDeleteBuffers(1, &m_BufferID); }
// ------------------------------------------------------------------------------------------
void GLBuffer::Bind() const { glBindBuffer(m_Target, m_BufferID); }
// ------------------------------------------------------------------------------------------
void GLBuffer::Unbind() const { glBindBuffer(m_Target, 0); }
// ------------------------------------------------------------------------------------------
GLuint GLBuffer::GetID() const { return m_BufferID; }
// ------------------------------------------------------------------------------------------
GLBuffer::operator u32() const { return m_BufferID; }
// ------------------------------------------------------------------------------------------
GLBuffer::~GLBuffer() { Release(); }
