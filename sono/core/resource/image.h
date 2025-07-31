#ifndef SN_IMAGE_H
#define SN_IMAGE_H

#include "core/common/types.h"
#include <stb_image.h>
#include <string>

class Image {
public:
  Image();

  Image(u32 width, u32 height, u32 channels, u8 *data, b8 autoFree);

  Image(const std::string &imageFile);

  ~Image();

  void Save(const std::string &fname) const;

  void FreeBuffer();

  void LoadFromBytes(u8 *data, u32 width, u32 height, u32 channels, b8 autoFree);

  void LoadFromFile(const std::string &imageFile);

  inline u32 GetWidth() const { return m_Width; }

  inline u32 GetHeight() const { return m_Height; }

private:
  usize CalculateBufSize(u32 width, u32 height, u32 channels) const;

private:
  u8 *m_Buffer;
  usize m_BufSize;
  u32 m_Width;
  u32 m_Height;
  u32 m_Channels;
  b8 m_AutoFree;
};

#endif // !SN_IMAGE_H
