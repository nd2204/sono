#ifndef SN_GL_BUFFER_MANAGER_H
#define SN_GL_BUFFER_MANAGER_H

#include <render/render_device.h>
#include <render/shader/shader.h>
#include <glad/glad.h>

class GLRenderDevice : public RenderDevice {
public:
  using RenderDevice::RenderDevice;

  void Init() override;

  void Shutdown() override;

  RenderPipeline *CreateDefaultPipeline() override;

  RenderPipeline *CreatePipeline(const PipelineDesc &desc) override;

  Shader *CreateShader(const ShaderDesc &desc) override;

  Texture *CreateTexture(
    TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
  ) override;

  CommandList *CreateCommandList() override;

  Buffer *CreateBuffer(const BufferDesc &desc) override;

  b8 DeleteBuffer(Buffer *pBuf) override;
};

// ------------------------------------------------------------------------------------------

#endif // !SN_GL_BUFFER_MANAGER_H
