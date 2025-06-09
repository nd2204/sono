#include "glvbo.h"
#include "core/common/snassert.h"

GLVertexBuffer::GLVertexBuffer(const void *data, u32 size, u32 usage) {
  glGenBuffers(1, &m_ID);
  SetBufferData(data, size, usage);
}

GLVertexBuffer::GLVertexBuffer() { glGenBuffers(1, &m_ID); }

void GLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_ID); }

void GLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void GLVertexBuffer::SetBufferData(const void *data, u32 size, u32 usage) {
  SN_ASSERT(data != nullptr, "VBO data cannot be null");
  m_Size = size;
  glBindBuffer(GL_ARRAY_BUFFER, m_ID);
  glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

u32 GLVertexBuffer::GetID() const { return m_ID; }

GLVertexBuffer::operator u32() const { return m_ID; }

GLVertexBuffer::~GLVertexBuffer() { glDeleteBuffers(1, &m_ID); }