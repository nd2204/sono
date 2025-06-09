#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "core/common/types.h"
#include "glad/glad.h"

class GLTexture {
public:
  GLTexture(u32 target, u32 internalFormat, i32 width, i32 height);

  u32 GetID() const;

  void Bind(const u32 unit = 0) const;

  void Unbind() const;

  void SetData(const void *data, u32 format, u32 type);

  void SetWrap(u32 wrapMode);

  void SetWrapAxis(u32 wrapAxis, u32 wrapMode);

  void SetFilter(u32 filterMode);

  void SetFilterZoom(u32 filterZoom, u32 filterMode);

  void GenerateMipmap() const;

  operator u32() const;

  ~GLTexture();

private:
  u32 m_TextureID; // OpenGL texture ID
  u32 m_Width;     // Texture width
  u32 m_Height;    // Texture height
  u32 m_Target;    // Texture type (e.g., GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP)
  u32 m_InternalFormat; // Internal format (e.g., GL_RGBA)
  // u32 m_Channels;       // Number of channels (3 for RGB, 4 for RGBA)
};

#endif // !GLTEXTURE_H