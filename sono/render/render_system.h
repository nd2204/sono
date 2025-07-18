#ifndef SN_RENDER_SYSTEM_H
#define SN_RENDER_SYSTEM_H

#include "camera.h"
#include "core/system.h"
#include "core/common/singleton.h"
#include "core/memory/allocators/arena.h"
#include "render/render_command.h"
#include "render/render_context.h"
#include "render/buffer/buffer_manager.h"
#include "render/shader/shader.h"
#include "render/resource/texture.h"

#include "render_queue.h"
#include "render_window.h"
#include "vertex_array.h"
#include "vertex_type.h" // IWYU pragma: export

#include <imgui.h>

struct BufferDesc;

class RenderSystem
  : public Singleton<RenderSystem>
  , public ISystem {
public:
  RenderSystem();

  virtual ~RenderSystem();

  virtual RenderWindow *CreateRenderWindow(
    i32 width, i32 height, const char *Title, WindowMode mode = WIN_MODE_WINDOWED
  );

  // ================================================================================
  // Render operations
  // ================================================================================

  virtual void BeginFrame(const Camera &cam) = 0;

  virtual void EndFrame() = 0;

  virtual void Present() = 0;

  template <class Cmd, typename... Args>
  void Submit(Args &&...args) {
    auto *cmd = m_Arena.New<Cmd>(std::forward<Args>(args)...);
    if (cmd) {
      m_RenderQueue.Submit(cmd);
      return;
    }
    LOG_WARN("cannot allocate memory for command, command unsubmitted");
  };

  virtual void SetViewport(i32 posX, i32 posY, i32 width, i32 height) = 0;

  virtual void Draw(PrimitiveType topology, const VertexArray *va, u32 maxVertCount) = 0;

  virtual void DrawIndexed(PrimitiveType topology, const VertexArray *va, u32 idxCount) = 0;

  virtual void Clear(const Vec4 &color) = 0;

  virtual void Flush();

  // ================================================================================
  // Pipeline management
  // ================================================================================

  virtual Shader *CreateShader(const ShaderDesc *desc = nullptr) = 0;

  virtual VertexArray *CreateVertexArray() = 0;

  virtual void BindVertexArray(VertexArray *va) = 0;

  virtual Texture *CreateTexture(
    TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
  ) = 0;

  virtual RenderPipeline *CreatePipeline(Shader *vs, Shader *fs) = 0;

  virtual void BindTexture(Texture *texture, u32 unit) = 0;

  virtual void BindBuffer(IBuffer *buffer, u32 index) = 0;

  virtual void BindPipeline(RenderPipeline *pipeline, u32 index) = 0;

  virtual void UnbindPipeline(RenderPipeline *pipeline, u32 index) = 0;

  // ================================================================================
  // ImGui
  // ================================================================================

  virtual void InitImGui(RenderWindow *window) = 0;

  virtual void ShutdownImGui() = 0;

  virtual void BeginImGuiFrame() = 0;

  virtual void EndImGuiFrame() = 0;

  // ================================================================================
  // Getters & Setters
  // ================================================================================

  virtual void SetRenderContext(RenderContext *ctx) = 0;

  const ArenaAllocator &GetFrameAllocator() const { return m_Arena; }

  RenderContext *GetCurrentContext() { return m_pActiveCtx; };

  RenderPipeline *GetCurrentPipeline() { return m_pActivePipeline; }

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
