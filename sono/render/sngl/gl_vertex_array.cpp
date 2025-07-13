#include "gl_buffer_base.h"
#include "gl_index_buffer.h"
#include "gl_vertex_buffer.h"
#include "gl_vertex_array.h"
#include "core/common/snassert.h"

#define GL_ATTRIBUTE_COUNT 16

static GLenum ConvertToGLType(VertexAttribType type) {
  switch (type) {
    case VAT_FLOAT:
    case VAT_FLOAT2:
    case VAT_FLOAT3:
    case VAT_FLOAT4:
      return GL_FLOAT;
    case VAT_INT:
    case VAT_INT2:
    case VAT_INT3:
    case VAT_INT4:
      return GL_INT;
    case VAT_UINT:
    case VAT_UINT2:
    case VAT_UINT3:
    case VAT_UINT4:
      return GL_UNSIGNED_INT;
    case VAT_BYTE4_NORM:
    case VAT_BYTE4_SNORM:
      return GL_BYTE;
    case VAT_USHORT2_NORM:
    case VAT_USHORT4_NORM:
      return GL_UNSIGNED_SHORT;
    case VAT_HALF2:
    case VAT_HALF4:
      return GL_UNSIGNED_SHORT;
  }

  SN_ASSERT(false, "Unsupported VertexAttribType");
  return 0;
}

// ===============================================================================
// GLVertexArray implementation
// ===============================================================================

GLVertexArray::GLVertexArray()
  : m_pIndexBuffer(nullptr) {
  glGenVertexArrays(1, &m_ID);
}
// --------------------------------------------------------------------------------
void GLVertexArray::Bind() const { glBindVertexArray(m_ID); }
// --------------------------------------------------------------------------------
void GLVertexArray::Unbind() const { glBindVertexArray(0); }
// --------------------------------------------------------------------------------
void GLVertexArray::AddVertexBuffer(IBuffer *buffer, const VertexLayout &layout) {
  ASSERT(buffer);

  glBindVertexArray(m_ID);
  buffer->Bind();

  LOG_DEBUG_F("__ Vertex Array [id=%d]", m_ID);
  const auto &attributes = layout.GetAttributes();
  for (auto &attrib : attributes) {
    LOG_DEBUG_F(
      "   |__ enabling vertex attributes [loc=%d, count=%d, type=%d, normalized=%d, stride=%d, "
      "offset=%d]",
      attrib.location, attrib.GetElementCount(), attrib.type, attrib.normalized, layout.GetStride(),
      attrib.offset
    );
    glEnableVertexAttribArray(attrib.location);
    glVertexAttribPointer(
      attrib.location, attrib.GetElementCount(), ConvertToGLType(attrib.type),
      attrib.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
      reinterpret_cast<void *>(attrib.offset)
    );
  }

  m_VertexBuffers.push_back(static_cast<GLVertexBuffer *>(buffer));
}
// --------------------------------------------------------------------------------
void GLVertexArray::SetIndexBuffer(IBuffer *buffer) {
  ASSERT(buffer);
  glBindVertexArray(m_ID);
  m_pIndexBuffer = static_cast<GLIndexBuffer *>(buffer);
  m_pIndexBuffer->Bind();
}
// --------------------------------------------------------------------------------
GLuint GLVertexArray::GetID() const { return m_ID; }
// --------------------------------------------------------------------------------
const GLIndexBuffer *GLVertexArray::GetCurrentIndexBuffer() const { return m_pIndexBuffer; }
// --------------------------------------------------------------------------------
const std::vector<GLVertexBuffer *> &GLVertexArray::GetVertexBuffers() const {
  return m_VertexBuffers;
}
// --------------------------------------------------------------------------------
GLVertexBuffer *GLVertexArray::operator[](usize index) { return m_VertexBuffers[index]; }
// --------------------------------------------------------------------------------
const GLVertexBuffer *GLVertexArray::operator[](usize index) const {
  return m_VertexBuffers[index];
}
// --------------------------------------------------------------------------------
GLVertexArray::~GLVertexArray() {
  if (m_ID) glDeleteVertexArrays(1, &m_ID);
}
