#include "gltexture.h"
#include "core/common/snassert.h"
#include "glad/glad.h"

GLTexture::GLTexture(u32 target, u32 internalFormat, i32 width, i32 height)
  : m_TextureID(0)
  , m_Width(width)
  , m_Height(height)
  , m_Target(target)
  , m_InternalFormat(internalFormat) {
  glGenTextures(1, &m_TextureID);
  glBindTexture(target, m_TextureID);
  // glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLTexture::~GLTexture() { glDeleteTextures(1, &m_TextureID); }

void GLTexture::SetData(const void *data, u32 format, u32 type) {
  SN_ASSERT(data, "Texture data cannot be null");
  glBindTexture(m_Target, m_TextureID);
  glTexImage2D(
    m_Target,
    0,
    m_InternalFormat,
    m_Width,
    m_Height,
    0,
    format,
    type,
    data
  );
}

void GLTexture::Bind(const u32 unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_Target, m_TextureID);
}

void GLTexture::GenerateMipmap() const {
  glBindTexture(m_Target, m_TextureID);
  glGenerateMipmap(m_Target);
}

void GLTexture::SetWrap(u32 wrapMode) {
  glBindTexture(m_Target, m_TextureID);
  glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, wrapMode);
  glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, wrapMode);
}

void GLTexture::SetWrapAxis(u32 wrapAxis, u32 wrapMode) {
  glBindTexture(m_Target, m_TextureID);
  glTexParameteri(m_Target, wrapAxis, wrapMode);
}

void GLTexture::SetFilter(u32 filterMode) {
  glBindTexture(m_Target, m_TextureID);
  glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, filterMode);
  glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, filterMode);
}

void GLTexture::SetFilterZoom(u32 filterZoom, u32 filterMode) {
  glBindTexture(m_Target, m_TextureID);
  glTexParameteri(m_Target, filterZoom, filterMode);
}

void GLTexture::Unbind() const { glBindTexture(m_Target, 0); }

GLTexture::operator u32() const { return m_TextureID; }