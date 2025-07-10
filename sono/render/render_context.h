#ifndef SN_RENDER_CONTEXT_H
#define SN_RENDER_CONTEXT_H

#include "core/common/types.h"

#include <GLFW/glfw3.h>

class RenderContext {
public:
  RenderContext(u32 width, u32 height);

  virtual ~RenderContext();

  /// @return the width of the rendering context
  u32 GetWidth() const;

  /// @return the height of the rendering context
  u32 GetHeight() const;

  /// @return the context's aspect ratio in float
  f32 GetAspect() const;

  virtual void MakeCurrent() = 0;

  /// @brief swap front and back buffer
  virtual void SwapBuffers() = 0;

protected:
  u32 m_Width, m_Height;
};

#endif // !SN_RENDER_CONTEXT_H
