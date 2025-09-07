#ifndef SN_GL_RENDER_SYSTEM_H
#define SN_GL_RENDER_SYSTEM_H

#include <render/render_system.h>
#include <core/math/mat4.h>

class GLRenderSystem : public RenderSystem {
public:
  GLRenderSystem();

  void Init() override;

  void Shutdown() override;

  const char *GetName() const override { return "GLRenderSystem"; }

  RenderWindow *CreateRenderWindow(
    i32 width, i32 height, const char *title, WindowMode mode = WIN_MODE_WINDOWED
  ) override;

  // ================================================================================
  // Render operations
  // ================================================================================

  void BeginFrame(const Camera &cam) override;

  void EndFrame() override;

  void Present() override;

  void SetViewport(i32 posX, i32 posY, i32 width, i32 height) override;

  void Draw(PrimitiveType topology, const VertexArray *va, u32 offset, u32 mMaxVertCount) override;

  void DrawIndexed(
    PrimitiveType topology, const VertexArray *va, u32 idxOffset, u32 idxCount
  ) override;

  void Clear(const Vec4 &color) override;

  // ================================================================================
  // ImGui
  // ================================================================================

  void InitImGui(RenderWindow *window) override;

  void ShutdownImGui() override;

  void BeginImGuiFrame() override;

  void EndImGuiFrame() override;

  // ================================================================================
  // Pipeline management
  // ================================================================================

  void BindVertexArray(VertexArray *va) override;

  void BindTexture(Texture *texture, u32 unit) override;

  void BindBuffer(Buffer *buffer, u32 index) override;

  void BindPipeline(RenderPipeline *pipeline, u32 index) override;

  // ================================================================================
  // Getters & Setters
  // ================================================================================

  void SetRenderContext(RenderContext *renderContext) override;

public:
  static GLenum ConvertPrimitiveType(PrimitiveType type);

private:
};

#endif // !SN_GL_RENDER_SYSTEM_H
