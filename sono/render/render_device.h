#ifndef SN_BUFFER_MANAGER_H
#define SN_BUFFER_MANAGER_H

#include "core/common/singleton.h"

#include "core/memory/allocator.h"
#include "render/buffer_base.h"
#include "render/resource/texture.h"
#include "render/shader/shader.h"

#include <set>

class RenderDevice : public Singleton<RenderDevice> {
public:
  RenderDevice(Allocator *allocator);

  virtual ~RenderDevice();

  virtual RenderPipeline *CreateDefaultPipeline() = 0;

  virtual RenderPipeline *CreatePipeline(const PipelineDesc &desc) = 0;

  virtual Shader *CreateShader(const ShaderDesc &desc) = 0;

  virtual Texture *CreateTexture(
    TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
  ) = 0;

  virtual Buffer *CreateBuffer(const BufferDesc &desc) = 0;

  virtual b8 DeleteBuffer(Buffer *pBuf) = 0;

  virtual void DeleteAllBuffers();

protected:
  std::set<Buffer *> m_Buffers;
  Allocator *m_pAllocator;
};

#endif // !SN_BUFFER_MANAGER_H
