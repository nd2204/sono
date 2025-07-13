#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "render/resource/texture.h"
#include "core/common/types.h"
#include "glad/glad.h"

class GLTexture : public Texture {
public:
  GLTexture(
    TextureType type, TextureFormat internalFormat, TextureFormat format, i32 width, i32 height
  );

  u32 GetID() const;

  void Bind(const u32 unit = 0) const;

  void Unbind() const;

  void SetWrap(u32 wrapMode);

  void SetWrapAxis(u32 wrapAxis, u32 wrapMode);

  void SetFilter(u32 filterMode);

  void SetFilterZoom(u32 filterZoom, u32 filterMode);

  virtual void GenerateMipmaps() override;

  virtual void *GetNativeHandle() override;

  virtual void Update(const void *data, u32 mipLevels) override;

  virtual void SetFormat(TextureFormat format) override;

  operator u32() const;

  ~GLTexture();

private:
  u32 m_TextureID; // OpenGL texture ID
  // u32 m_Channels;              // Number of channels (3 for RGB, 4 for RGBA)
};

#endif // !GLTEXTURE_H
