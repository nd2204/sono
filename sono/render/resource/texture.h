#ifndef SN_TEXTURE_H
#define SN_TEXTURE_H

#include <core/common/defines.h>
#include <core/resource/resource_base.h>
#include <core/common/types.h>
#include <core/math/vec3.h>

enum class TextureFormat : u8 {
  // 8-bit formats
  r8_unorm,
  r8_snorm,
  r8_uint,
  r8_sint,
  // 16-bit formats
  r16_unorm,
  r16_snorm,
  r16_uint,
  r16_sint,
  r16_float,
  rg8_unorm,
  rg8_snorm,
  rg8_uint,
  rg8_sint,
  // 24-bit formats
  rgb8_unorm,
  // 32-bit formats
  r32_uint,
  r32_sint,
  r32_float,
  rg16_unorm,
  rg16_snorm,
  rg16_uint,
  rg16_sint,
  rg16_float,
  rgba8_unorm,
  rgba8_unorm_srgb,
  rgba8_snorm,
  rgba8_uint,
  rgba8_sint,
  bgra8_unorm,
  bgra8_unorm_srgb,
  // Packed 32-bit formats
  rgb9e5_ufloat,
  rgb10a2_uint,
  rgb10a2_unorm,
  rg11b10_ufloat,
  // 64-bit formats
  rg32_uint,
  rg32_sint,
  rg32_float,
  rgba16_unorm,
  rgba16_snorm,
  rgba16_uint,
  rgba16_sint,
  rgba16_float,
  // 128-bit formats
  rgba32_uint,
  rgba32_sint,
  rgba32_float,
  // Depth/stencil formats
  stencil8,
  depth16_unorm,
  depth24_plus,
  depth24_plus_stencil8,
  depth32_float,
  // "depth32float-stencil8" feature
  depth32float_stencil8,
};

enum class TextureUsage : u8 {
  // clang-format off
  TextureBinding = 0,
  StorageBinding = BITVAL(1),
  RenderTarget   = BITVAL(2),
  // Texture can be use as read source
  CopySrc        = BITVAL(3),
  // Texture can be use as write destination
  CopyDest       = BITVAL(4)
  // clang-format on
};

inline TextureUsage operator|(TextureUsage lhs, TextureUsage rhs) {
  return static_cast<TextureUsage>(static_cast<u32>(lhs) | static_cast<u32>(rhs));
}

inline u32 operator&(TextureUsage lhs, TextureUsage rhs) {
  return static_cast<u32>(lhs) & static_cast<u32>(rhs);
}

enum class TextureType : u16 {
  Texture1D,
  Texture2D,
  Texture3D,
  TextureCubeMap,
};

struct TextureDesc {
  u32 size[3];
  TextureFormat format;
  TextureUsage usage;
  TextureType type = TextureType::Texture2D;
  u32 mipLevels = 1;
};

class Texture /*: public ResourceBase<Texture>*/ {
public:
  Texture(const TextureDesc &desc)
    : m_Desc(desc) {}

  virtual ~Texture() = default;

  virtual void GenerateMipmaps() = 0;

  virtual void *GetNativeHandle() = 0;

  virtual void Update(const void *data, u32 mipLevels) = 0;

  u32 GetWidth() const { return m_Desc.size[0]; };

  u32 GetHeight() const { return m_Desc.size[1]; };

  u32 GetDepth() const { return m_Desc.size[2]; };

  u32 GetMipLevels() const { return m_Desc.mipLevels; };

  TextureType GetType() const { return m_Desc.type; };

  TextureFormat GetFormat() const { return m_Desc.format; };

protected:
  TextureDesc m_Desc;
};

#endif // !SN_TEXTURE_H
