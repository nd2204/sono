#ifndef SN_GL_RENDER_SYSTEM_H
#define SN_GL_RENDER_SYSTEM_H

#include "render/render_system.h"
#include "core/math/mat4.h"

class GLRenderSystem : public RenderSystem {
public:
  GLRenderSystem();

  void Init() override;

  void Shutdown() override;

  RenderWindow *CreateRenderWindow(
    i32 width, i32 height, const char *title, WindowMode mode = WIN_MODE_WINDOWED
  ) override;

  // ================================================================================
  // Render operations
  // ================================================================================

  void BeginFrame() override;

  void EndFrame() override;

  void Present() override;

  void SetViewport(i32 posX, i32 posY, i32 width, i32 height) override;

  void Draw(PrimitiveType topology, VertexArray *va, u32 mMaxVertCount) override;

  void DrawIndexed(PrimitiveType topology, VertexArray *va, u32 idxCount) override;

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

  VertexArray *CreateVertexArray() override;

  void BindVertexArray(VertexArray *va) override;

  Shader *CreateShader(const ShaderDesc *desc) override;

  Texture *CreateTexture(
    TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
  ) override;

  RenderPipeline *CreatePipeline(Shader *vs, Shader *fs) override;

  void BindTexture(Texture *texture, u32 unit) override;

  void BindBuffer(IBuffer *buffer, u32 index) override;

  void BindPipeline(RenderPipeline *pipeline, u32 index) override;

  void UnbindPipeline(RenderPipeline *pipeline, u32 index) override;

  // ================================================================================
  // Getters & Setters
  // ================================================================================

  void SetRenderContext(RenderContext *renderContext) override;

public:
  static GLenum ConvertPrimitiveType(PrimitiveType type);

private:
  // Mat4 m_Model;
  // Mat4 m_View;
};

#endif // !SN_GL_RENDER_SYSTEM_H
