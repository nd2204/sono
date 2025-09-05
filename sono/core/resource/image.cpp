#include "image.h"
#include "core/common/logger.h"
#include "core/memory/memory_system.h"
#include <cstring>
#include <fstream>

Image::Image()
  : m_Buffer(nullptr)
  , m_BufSize(0)
  , m_Width(0)
  , m_Height(0)
  , m_Channels(0)
  , m_AutoFree(true) {}
// --------------------------------------------------------------------------------
Image::Image(const std::string &imageFile)
  : Image() {
  LoadFromFile(imageFile);
}
// --------------------------------------------------------------------------------
Image::Image(u32 width, u32 height, u32 channels, u8 *data, b8 autoFree)
  : Image() {
  usize bufSize = CalculateBufSize(width, height, channels);

  u8 *temp = data;
  if (!data) {
    temp = (u8 *)SN_ALLOC(bufSize, ALLOC_TYPE_RESOURCE);
  }

  LoadFromBytes(temp, width, height, channels, autoFree);
}
// --------------------------------------------------------------------------------
Image::~Image() { FreeBuffer(); }
// --------------------------------------------------------------------------------
void Image::Save(const std::string &fname) const {
  std::ofstream ofile(fname, std::ostream::binary);
  if (ofile.is_open()) {
    ofile.write((char *)m_Buffer, m_BufSize);
  } else {
    LOG_ERROR_F("Failed to open file %s", fname.c_str());
  }
}
// --------------------------------------------------------------------------------
void Image::FreeBuffer() {
  if (m_AutoFree) {
    SN_FREE(m_Buffer);
  }
}
// --------------------------------------------------------------------------------
usize Image::CalculateBufSize(u32 width, u32 height, u32 channels) const {
  return width * height * channels;
}
// --------------------------------------------------------------------------------
void Image::LoadFromFile(const std::string &imageFile) {
  int width, height, nrChannels;
  u8 *data;

  data = stbi_load(imageFile.c_str(), &width, &height, &nrChannels, 0);
  memcpy(m_Buffer, data, CalculateBufSize(width, height, nrChannels));
  stbi_image_free(data);

  LoadFromBytes(m_Buffer, width, height, nrChannels, true);
}
// --------------------------------------------------------------------------------
void Image::LoadFromBytes(u8 *data, u32 width, u32 height, u32 channels, b8 autoFree) {
  m_Buffer = data;
  m_BufSize = CalculateBufSize(width, height, channels);

  m_Width = width;
  m_Height = height;
  m_Channels = channels;
  m_AutoFree = autoFree;
}
