#include <render-backend/sngl/gl_texture.h>
#include <core/common/snassert.h>
#include <glad/glad.h>
#include <cstdint>

// clang-format off

static inline GLuint TextureTypeToTarget(TextureType type) {
  switch (type) {
    case TEX_TYPE_1D:       return GL_TEXTURE_1D;
    case TEX_TYPE_2D:       return GL_TEXTURE_2D;
    case TEX_TYPE_3D:       return GL_TEXTURE_3D;
    case TEX_TYPE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
  }
  SN_ASSERT(false, "OpenGL: Unsupported Texture Type");
  return 0;
}

static inline GLuint TextureFormatToGLFormat(TextureFormat format) {
  switch (format) {
    case TEX_FORMAT_RGB:    return GL_RGB;
    case TEX_FORMAT_RGBA:   return GL_RGBA;
  }
  SN_ASSERT(false, "OpenGL: Unsupported Texture Format");
  return 0;
}

// clang-format on

GLTexture::GLTexture(
  TextureType type, TextureFormat internalFormat, TextureFormat format, i32 width, i32 height
) {
  m_TextureID = 0;
  m_Width = width;
  m_Height = height;
  m_Depth = 0;
  m_Type = type;
  m_InternalFormat = internalFormat;
  m_Format = format;

  glGenTextures(1, &m_TextureID);
}

void GLTexture::Update(const void *data, u32 mipLevels) {
  SN_ASSERT(data, "Texture data cannot be null");

  m_MipLevels = mipLevels;
  GLuint target = TextureTypeToTarget(m_Type);

  glBindTexture(target, m_TextureID);
  glTexImage2D(
    target, mipLevels, TextureFormatToGLFormat(m_InternalFormat), m_Width, m_Height, 0,
    TextureFormatToGLFormat(m_Format), GL_UNSIGNED_BYTE, data
  );
}

void GLTexture::Bind(const u32 unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(TextureTypeToTarget(m_Type), m_TextureID);
}

void GLTexture::GenerateMipmaps() {
  GLuint target = TextureTypeToTarget(m_Type);
  glBindTexture(target, m_TextureID);
  glGenerateMipmap(target);
}

void *GLTexture::GetNativeHandle() {
  return reinterpret_cast<void *>(static_cast<uintptr_t>(m_TextureID));
}

void GLTexture::SetFormat(TextureFormat format) { m_Format = format; }

void GLTexture::SetWrap(u32 wrapMode) {
  GLuint target = TextureTypeToTarget(m_Type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapMode);
}

void GLTexture::SetWrapAxis(u32 wrapAxis, u32 wrapMode) {
  GLuint target = TextureTypeToTarget(m_Type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, wrapAxis, wrapMode);
}

void GLTexture::SetFilter(u32 filterMode) {
  GLuint target = TextureTypeToTarget(m_Type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterMode);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterMode);
}

void GLTexture::SetFilterZoom(u32 filterZoom, u32 filterMode) {
  GLuint target = TextureTypeToTarget(m_Type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, filterZoom, filterMode);
}

void GLTexture::Unbind() const { glBindTexture(TextureTypeToTarget(m_Type), 0); }

GLTexture::operator u32() const { return m_TextureID; }

GLTexture::~GLTexture() { glDeleteTextures(1, &m_TextureID); }
