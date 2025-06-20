#ifndef SN_GL_RENDER_SYSTEM_H
#define SN_GL_RENDER_SYSTEM_H

#include "../render_system.h"
#include "../render_op.h"
#include "core/math/mat4.h"

class GLRenderSystem : public RenderSystem {
public:
  void Init() override;

  void Shutdown() override;

  Window *CreateRenderWindow(
    i32 width, i32 height, const char *title, WindowMode mode = WIN_MODE_WINDOWED
  ) override;

  void SetRenderContext(RenderContext *renderContext) override;

  void Render(const RenderOp &op);

  void BeginFrame() override;

  void EndFrame() override;

private:
  Mat4 m_Model;
  Mat4 m_View;
};

#endif // !SN_GL_RENDER_SYSTEM_H
