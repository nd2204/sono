#include "glebo.h"

GLElementBuffer::GLElementBuffer(const void *data, u32 size, u32 usage) {
  glGenBuffers(1, &m_ID);
  SetBufferData(data, size, usage);
}

GLElementBuffer::GLElementBuffer() { glGenBuffers(1, &m_ID); }

void GLElementBuffer::SetBufferData(const void *data, u32 size, u32 usage) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
  m_Size = size;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

void GLElementBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void GLElementBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

u32 GLElementBuffer::GetID() const { return m_ID; }

u32 GLElementBuffer::GetSize() const { return m_Size; }

GLElementBuffer::operator u32() const { return m_ID; }

GLElementBuffer::~GLElementBuffer() { glDeleteBuffers(1, &m_ID); }