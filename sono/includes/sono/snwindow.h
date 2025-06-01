#ifndef SN_WINDOW_H
#define SN_WINDOW_H

#include "sono/types.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum WindowMode {
  WINDOWMODE_WINDOWED,
  WINDOWMODE_FULLSCREEN,
  WINDOWMODE_BORDERLESS
};

class SNWindow {
public:
  SNWindow(
    i32 width,
    i32 height,
    const char *Title,
    WindowMode mode = WINDOWMODE_WINDOWED
  );

  u32 GetWidth();
  u32 GetHeight();
  WindowMode GetCurrentWindowMode();
  // TODO: Turn this to a flag when things get bigger
  void EnableVsync(i32 vsync);
  void SetWindowMode(WindowMode mode);
  void SetFullScreen(i32 fullscreen);
  void ToggleFullScreen();

  // Convert this window object to GLFW window ptr
  operator GLFWwindow *() const;

private:
  GLFWwindow *m_Context;
  WindowMode m_Mode;
  u32 m_PosX, m_PosY;
  u32 m_Width, m_Height;
};

#endif // !SN_WINDOW_H