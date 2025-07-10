#include "gl_buffer_base.h"
#include "gl_buffer_manager.h"
#include "gl_index_buffer.h"
#include "gl_render_system.h"
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
  : m_pIndexBuffer(nullptr)
  , m_pVertexLayout(nullptr) {
  glGenVertexArrays(1, &m_ID);
}
// --------------------------------------------------------------------------------
void GLVertexArray::Bind() const {
  LOG_DEBUG_F("__ Vertex Array [id=%d]", m_ID);
  glBindVertexArray(m_ID);
  for (auto *vb : m_VertexBuffers) {
    LOG_DEBUG_F(" |__ binding vertex buffer [id=%d]", vb->GetID());
    vb->Bind();
    // Set vertex attribute pointers based on the layout
    const auto &attributes = m_pVertexLayout->GetAttributes();
    i32 size = attributes.size();
    for (i32 i = 0; i < size; ++i) {
      LOG_DEBUG_F(
        "   |__ enabling vertex attributes [loc=%d, count=%d, type=%d, normalized=%d, stride=%d, "
        "offset=%d]",
        attributes[i].location, attributes[i].GetElementCount(), attributes[i].type,
        attributes[i].normalized, m_pVertexLayout->GetStride(), attributes[i].offset
      );
      glVertexAttribPointer(
        attributes[i].location, attributes[i].GetElementCount(),
        ConvertToGLType(attributes[i].type), attributes[i].normalized ? GL_TRUE : GL_FALSE,
        m_pVertexLayout->GetStride(), reinterpret_cast<void *>(attributes[i].offset)
      );
      glEnableVertexAttribArray(attributes[i].location);
    }
  }
  if (m_pIndexBuffer) m_pIndexBuffer->Bind();
}
// --------------------------------------------------------------------------------
void GLVertexArray::Unbind() const { glBindVertexArray(0); }
// --------------------------------------------------------------------------------
void GLVertexArray::AddVertexBuffer(IBuffer *buffer) {
  ASSERT(buffer);
  m_VertexBuffers.push_back(static_cast<GLVertexBuffer *>(buffer));
}
// --------------------------------------------------------------------------------
void GLVertexArray::SetIndexBuffer(IBuffer *buffer) {
  ASSERT(buffer);
  m_pIndexBuffer = static_cast<GLIndexBuffer *>(buffer);
  glBindVertexArray(m_ID);
  m_pIndexBuffer->Bind();
}
// --------------------------------------------------------------------------------
void GLVertexArray::SetVertexLayout(VertexLayout *layout) {
  ASSERT(layout);
  m_pVertexLayout = layout;
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
GLVertexArray::~GLVertexArray() { glDeleteVertexArrays(1, &m_ID); }
