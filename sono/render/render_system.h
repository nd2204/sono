#ifndef SN_RENDER_SYSTEM_H
#define SN_RENDER_SYSTEM_H

#include "core/common/singleton.h"
#include "core/memory/allocators/arena.h"
#include "render_command.h"
#include "render_context.h"
#include "buffer_manager.h"

#include "render_queue.h"
#include "render_window.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

struct BufferDesc;

class RenderSystem : public Singleton<RenderSystem> {
public:
  RenderSystem();

  virtual ~RenderSystem();

  virtual void Init() = 0;

  virtual void Shutdown() = 0;

  virtual void SetRenderContext(RenderContext *ctx) = 0;

  virtual void BeginFrame() = 0;

  template <class Cmd, typename... Args>
  void Submit(Args &&...args) {
    auto *cmd = m_Arena.Create<Cmd>(std::forward<Args>(args)...);
    m_RenderQueue.Submit(cmd);
  };

  virtual void DrawIndexed(PrimitiveType topology, VertexArray *va, u32 idxCount) = 0;

  virtual void Clear(const Vec4 &color) = 0;

  virtual void Flush();

  virtual void EndFrame() = 0;

  virtual void Present() = 0;

  virtual Shader *CreateShader(const ShaderDesc *desc) = 0;

  virtual Texture *CreateTexture(
    TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
  ) = 0;

  virtual VertexArray *CreateVertexArray() = 0;

  virtual void BindVertexArray(VertexArray *va) = 0;

  virtual RenderPipeline *CreatePipeline(Shader *vs, Shader *fs) = 0;

  virtual void BindTexture(Texture *texture, u32 unit) = 0;

  virtual void BindBuffer(IBuffer *buffer, u32 index) = 0;

  virtual void BindPipeline(RenderPipeline *pipeline, u32 index) = 0;

  virtual void UnbindPipeline(RenderPipeline *pipeline, u32 index) = 0;

  virtual RenderWindow *CreateRenderWindow(
    i32 width, i32 height, const char *Title, WindowMode mode = WIN_MODE_WINDOWED
  );

  BufferManager *GetBufferManager() const;

protected:
  RenderContext *m_pActiveCtx;
  BufferManager *m_pBufferManager;
  RenderPipeline *m_pActivePipeline;
  RenderQueue m_RenderQueue;
  ArenaAllocator m_Arena;
  ArenaAllocator::Marker m_FrameBeginMark;
};

#endif // !SN_RENDER_SYSTEM_H
