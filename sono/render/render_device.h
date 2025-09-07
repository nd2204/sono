#ifndef SN_BUFFER_MANAGER_H
#define SN_BUFFER_MANAGER_H

#include <array>
#include <queue>
#include <render/command_queue.h>
#include <render/command_list.h>
#include <render/resource/texture.h>
#include <render/buffer_base.h>
#include <render/vertex_array.h>
#include <render/shader/shader.h>
#include <render/render_pipeline.h>
#include <core/system.h>
#include <core/memory/allocators/arena.h>
#include <core/common/singleton.h>
#include <core/memory/allocator.h>

#include <set>

using PipelineHandle = u32;

class RenderDevice
  : public Singleton<RenderDevice>
  , public System {
public:
  RenderDevice(Allocator *allocator);

  virtual ~RenderDevice();

  virtual RenderPipeline *CreateDefaultPipeline() = 0;

  virtual RenderPipeline *CreatePipeline(const PipelineDesc &desc) = 0;

  virtual Shader *CreateShader(const ShaderDesc &desc) = 0;

  virtual Texture *CreateTexture(const TextureDesc &desc) = 0;

  virtual Buffer *CreateBuffer(const BufferDesc &desc) = 0;

  virtual VertexArray *CreateVertexArray() = 0;

  virtual CommandList *CreateCommandList() = 0;

  virtual b8 DeleteBuffer(Buffer *pBuf) = 0;

  virtual void DeleteAllBuffers();

public:
  CommandQueue *queue;

protected:
  std::set<Buffer *> m_Buffers;

  // std::array<RenderPipeline *, 64> m_Pipelines;
  // std::queue<PipelineHandle> m_FreePipeline;

  Allocator *m_pResourceAllocator;
  ArenaAllocator m_FrameAllocator;
};

#endif // !SN_BUFFER_MANAGER_H
