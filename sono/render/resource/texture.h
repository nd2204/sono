#ifndef SN_TEXTURE_H
#define SN_TEXTURE_H

#include "core/common/types.h"

enum TextureFormat {
  TEX_FORMAT_RGB,
  TEX_FORMAT_RGBA,
};

enum TextureType {
  TEX_TYPE_1D,
  TEX_TYPE_2D,
  TEX_TYPE_3D,
  TEX_TYPE_CUBE_MAP,
};

class Texture {
public:
  virtual ~Texture() = default;

  virtual void GenerateMipmaps() = 0;

  virtual void *GetNativeHandle() = 0;

  virtual void Update(const void *data, u32 mipLevels) = 0;

  u32 GetWidth() { return m_Depth; };

  u32 GetHeight() { return m_Height; };

  u32 GetDepth() { return m_Depth; };

  u32 GetMipLevels() { return m_MipLevels; };

  TextureType GetType() { return m_Type; };

  TextureFormat GetFormat() { return m_Format; };

  virtual void SetFormat(TextureFormat tf) = 0;

protected:
  u32 m_Width;
  u32 m_Height;
  u32 m_Depth;
  u32 m_MipLevels;

  TextureFormat m_InternalFormat;
  TextureFormat m_Format;

  TextureType m_Type; // Texture type (e.g., GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP)
};

#endif // !SN_TEXTURE_H
