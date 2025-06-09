#include "glvao.h"
#include "core/common/logger.h"
#include "core/common/snassert.h"
#include "glvbo.h"

#define GL_ATTRIBUTE_COUNT 16

// ===============================================================================
// GLVertexLayout implementation
// ===============================================================================

u32 GLVertexLayout::s_MaxAttributeCount = GL_ATTRIBUTE_COUNT;

GLVertexLayout::GLVertexLayout()
  : m_Stride(0) {
  m_Attributes.reserve(GL_ATTRIBUTE_COUNT);
}

void GLVertexLayout::Push(const GLVertexAttribute &attribute) {
  if (m_Attributes.size() >= s_MaxAttributeCount) {
    LOG_WARN("GLVertexLayout: Maximum attribute count exceeded.");
    return;
  }
  m_Stride += attribute.count * attribute.tsize;
  m_Attributes.push_back(attribute);
}

void GLVertexLayout::Push(GLVertexAttribute &&attribute) {
  if (m_Attributes.size() >= s_MaxAttributeCount) {
    LOG_WARN("GLVertexLayout: Maximum attribute count exceeded.");
    return;
  }
  m_Stride += attribute.count * attribute.tsize;
  m_Attributes.emplace_back(attribute);
}

u32 GLVertexLayout::GetStride() const { return m_Stride; }

const std::vector<GLVertexAttribute> &GLVertexLayout::GetAttributes() const {
  return m_Attributes;
}

// ===============================================================================
// GLVertexArray implementation
// ===============================================================================

GLVertexArray::GLVertexArray() { glGenVertexArrays(1, &m_ID); }

void GLVertexArray::Bind() const { glBindVertexArray(m_ID); }

void GLVertexArray::Unbind() const { glBindVertexArray(0); }

void GLVertexArray::SetVertexBuffer(
  const GLVertexBuffer &vbo, const GLVertexLayout &layout
) {
  glBindVertexArray(m_ID);
  vbo.Bind();
  // Set vertex attribute pointers based on the layout
  const auto &attributes = layout.GetAttributes();
  u64 curOffset = 0;
  i32 size = attributes.size();
  for (i32 i = 0; i < size; ++i) {
    glVertexAttribPointer(
      i,
      attributes[i].count,
      attributes[i].type,
      attributes[i].normalized,
      layout.GetStride(),
      (void *)curOffset
    );
    curOffset += attributes[i].count * attributes[i].tsize;
    glEnableVertexAttribArray(i);
  }
}

void GLVertexArray::SetELementBuffer(const GLElementBuffer &ebo) {
  m_pEBO = std::make_shared<GLElementBuffer>(ebo);
  glBindVertexArray(m_ID);
  ebo.Bind();
}

void GLVertexArray::Draw(u32 mode) {
  SN_ASSERT(m_pEBO, "GLVertexArray: No EBO bound for drawing.");
  glBindVertexArray(m_ID);
  m_pEBO->Bind();
  glDrawElements(mode, m_pEBO->GetSize() / sizeof(u32), GL_UNSIGNED_INT, 0);
}

GLVertexArray::~GLVertexArray() { glDeleteVertexArrays(1, &m_ID); }