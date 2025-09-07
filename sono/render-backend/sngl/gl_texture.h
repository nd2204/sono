#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "render/resource/texture.h"
#include "core/common/types.h"
#include "glad/glad.h"

class GLTexture : public Texture {
public:
  GLTexture(const TextureDesc &desc);

  u32 GetID() const;

  void Bind(const u32 unit = 0) const;

  void Unbind() const;

  void SetWrap(GLuint wrapMode);

  void SetWrapAxis(GLuint wrapAxis, GLuint wrapMode);

  void SetFilter(GLuint filterMode);

  void SetFilterZoom(GLuint filterZoom, GLuint filterMode);

  virtual void GenerateMipmaps() override;

  virtual void *GetNativeHandle() override;

  virtual void Update(const void *data, u32 mipLevels) override;

  operator u32() const;

  ~GLTexture();

private:
  u32 m_TextureID; // OpenGL texture ID
  GLuint m_Target;
};

#endif // !GLTEXTURE_H
