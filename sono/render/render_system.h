#ifndef SN_RENDER_SYSTEM_H
#define SN_RENDER_SYSTEM_H

#include <core/system.h>
#include <core/common/singleton.h>
#include <core/memory/allocators/arena.h>
#include <render/camera.h>
#include <render/render_command.h>
#include <render/render_context.h>
#include <render/render_device.h>
#include <render/resource/texture.h>
#include <render/render_queue.h>
#include <render/render_window.h>
#include <render/vertex_array.h>
#include <render/vertex_type.h> // IWYU pragma: export
#include <render/debug/debug_draw.h>
#include <imgui.h>

class RenderSystem
  : public Singleton<RenderSystem>
  , public System {
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

  virtual void Draw(
    PrimitiveType topology, const VertexArray *va, u32 offset, u32 maxVertCount
  ) = 0;

  virtual void DrawIndexed(
    PrimitiveType topology, const VertexArray *va, u32 idxOffset, u32 idxCount
  ) = 0;

  virtual void Clear(const Vec4 &color) = 0;

  virtual void Flush();

  // ================================================================================
  // Pipeline management
  // ================================================================================

  virtual void BindVertexArray(VertexArray *va) = 0;

  virtual void BindTexture(Texture *texture, u32 unit) = 0;

  virtual void BindBuffer(Buffer *buffer, u32 index) = 0;

  virtual void BindPipeline(RenderPipeline *pipeline, u32 index) = 0;

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

  inline const ArenaAllocator &GetFrameAllocator() const { return m_Arena; }

  inline RenderContext *GetCurrentContext() { return m_pActiveCtx; };

  inline RenderPipeline *GetCurrentPipeline() { return m_pActivePipeline; }

  inline RenderDevice *GetRenderDevice() const { return m_pDevice; };

protected:
  RenderContext *m_pActiveCtx;
  RenderDevice *m_pDevice;
  RenderPipeline *m_pActivePipeline;
  DebugDraw *m_DebugDraw;
  // TODO: remove this after finishing command queue on render device
  RenderQueue m_RenderQueue;
  ArenaAllocator m_Arena;
  ArenaAllocator::Marker m_FrameBeginMark;
};

#endif // !SN_RENDER_SYSTEM_H
