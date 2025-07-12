#ifndef SN_GL_WINDOW_H
#define SN_GL_WINDOW_H

#include "render/render_window.h"

class GLWindow : public RenderWindow {
public:
  GLWindow();

  ~GLWindow();

  void Create(
    i32 width, i32 height, const char *Title, WindowMode mode = WIN_MODE_WINDOWED
  ) override;

  void Destroy() override;

private:
};

#endif // !SN_GL_WINDOW_H
