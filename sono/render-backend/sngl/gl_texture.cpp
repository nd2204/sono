#include <render-backend/sngl/gl_texture.h>
#include <core/common/snassert.h>
#include <glad/glad.h>
#include <cstdint>

static inline GLuint TextureTypeToTarget(TextureType type) {
  // clang-format off
  switch (type) {
    case TextureType::Texture1D:       return GL_TEXTURE_1D;
    case TextureType::Texture2D:       return GL_TEXTURE_2D;
    case TextureType::Texture3D:       return GL_TEXTURE_3D;
    case TextureType::TextureCubeMap:  return GL_TEXTURE_CUBE_MAP;
  }
  // clang-format on
  SN_ASSERT(false, "OpenGL: Unsupported Texture Type");
  return 0;
}

static inline GLuint TextureFormatToGLFormat(TextureFormat format) {
  // clang-format off
  switch (format) {
    case TextureFormat::rgb8_unorm:  return GL_RGB;
    case TextureFormat::rgba8_unorm: return GL_RGBA;
    case TextureFormat::r8_unorm:
    case TextureFormat::r8_snorm:
    case TextureFormat::r8_uint:
    case TextureFormat::r8_sint:
    case TextureFormat::r16_unorm:
    case TextureFormat::r16_snorm:
    case TextureFormat::r16_uint:
    case TextureFormat::r16_sint:
    case TextureFormat::r16_float:
    case TextureFormat::rg8_unorm:
    case TextureFormat::rg8_snorm:
    case TextureFormat::rg8_uint:
    case TextureFormat::rg8_sint:
    case TextureFormat::r32_uint:
    case TextureFormat::r32_sint:
    case TextureFormat::r32_float:
    case TextureFormat::rg16_unorm:
    case TextureFormat::rg16_snorm:
    case TextureFormat::rg16_uint:
    case TextureFormat::rg16_sint:
    case TextureFormat::rg16_float:
    case TextureFormat::rgba8_unorm_srgb:
    case TextureFormat::rgba8_snorm:
    case TextureFormat::rgba8_uint:
    case TextureFormat::rgba8_sint:
    case TextureFormat::bgra8_unorm:
    case TextureFormat::bgra8_unorm_srgb:
    case TextureFormat::rgb9e5_ufloat:
    case TextureFormat::rgb10a2_uint:
    case TextureFormat::rgb10a2_unorm:
    case TextureFormat::rg11b10_ufloat:
    case TextureFormat::rg32_uint:
    case TextureFormat::rg32_sint:
    case TextureFormat::rg32_float:
    case TextureFormat::rgba16_unorm:
    case TextureFormat::rgba16_snorm:
    case TextureFormat::rgba16_uint:
    case TextureFormat::rgba16_sint:
    case TextureFormat::rgba16_float:
    case TextureFormat::rgba32_uint:
    case TextureFormat::rgba32_sint:
    case TextureFormat::rgba32_float:
    case TextureFormat::stencil8:
    case TextureFormat::depth16_unorm:
    case TextureFormat::depth24_plus:
    case TextureFormat::depth24_plus_stencil8:
    case TextureFormat::depth32_float:
    case TextureFormat::depth32float_stencil8:
      break;
  }
  // clang-format on
  SN_ASSERT(false, "OpenGL: Unsupported Texture Format");
  return 0;
}

GLTexture::GLTexture(const TextureDesc &desc)
  : Texture(desc)
  , m_TextureID(0)
  , m_Target(TextureTypeToTarget(desc.type)) {
  glGenTextures(1, &m_TextureID);
}

void GLTexture::Update(const void *data, u32 mipLevels) {
  SN_ASSERT(data, "Texture data cannot be null");

  m_Desc.mipLevels = mipLevels;
  glBindTexture(m_Target, m_TextureID);

  GLuint format = TextureFormatToGLFormat(m_Desc.format);

  switch (m_Desc.type) {
    case TextureType::Texture1D:
      glTexImage1D(m_Target, mipLevels, format, GetWidth(), 0, format, GL_UNSIGNED_BYTE, data);
      break;
    case TextureType::Texture2D:
      glTexImage2D(
        m_Target, mipLevels, format, GetWidth(), GetHeight(), 0, format, GL_UNSIGNED_BYTE, data
      );
      break;
    case TextureType::Texture3D:
      glTexImage3D(
        m_Target, mipLevels, format, GetWidth(), GetHeight(), GetDepth(), 0, format,
        GL_UNSIGNED_BYTE, data
      );
      break;
    case TextureType::TextureCubeMap:
      break;
  }
}

void GLTexture::Bind(const u32 unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(TextureTypeToTarget(m_Desc.type), m_TextureID);
}

void GLTexture::GenerateMipmaps() {
  GLuint target = TextureTypeToTarget(m_Desc.type);
  glBindTexture(target, m_TextureID);
  glGenerateMipmap(target);
}

void *GLTexture::GetNativeHandle() {
  return reinterpret_cast<void *>(static_cast<uintptr_t>(m_TextureID));
}

void GLTexture::SetWrap(GLuint wrapMode) {
  GLuint target = TextureTypeToTarget(m_Desc.type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapMode);
}

void GLTexture::SetWrapAxis(GLuint wrapAxis, GLuint wrapMode) {
  GLuint target = TextureTypeToTarget(m_Desc.type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, wrapAxis, wrapMode);
}

void GLTexture::SetFilter(GLuint filterMode) {
  GLuint target = TextureTypeToTarget(m_Desc.type);
  glBindTexture(target, m_TextureID);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filterMode);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filterMode);
}

void GLTexture::SetFilterZoom(GLuint filterZoom, GLuint filterMode) {
  glBindTexture(m_Target, m_TextureID);
  glTexParameteri(m_Target, filterZoom, filterMode);
}

void GLTexture::Unbind() const { glBindTexture(m_Target, 0); }

GLTexture::operator u32() const { return m_TextureID; }

GLTexture::~GLTexture() { glDeleteTextures(1, &m_TextureID); }
