#ifndef SN_TEXTURE_MANAGER_H

#include "core/resource/resource_manager.h"
#include "core/resource/resource_handle.h"
#include "texture.h"

struct TagTexture {};

typedef ResourceHandle<TagTexture> TextureHandle;

struct TextureEntry {
  Texture *tex;
};

class TextureManager : public ResourceManager<TextureHandle, TextureEntry> {};

#endif // !SN_TEXTURE_MANAGER_H
