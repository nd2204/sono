#ifndef SN_WINDOW_CONTEXT_H
#define SN_WINDOW_CONTEXT_H

#include "GLFW/glfw3.h"
#include "render_context.h"

#include <memory>
#include <utility>

#include <functional>
#include <core/input/key_code.h>

enum WindowMode { WIN_MODE_WINDOWED, WIN_MODE_FULLSCREEN, WIN_MODE_BORDERLESS };

class RenderWindow : public RenderContext {
public:
  RenderWindow();

  virtual ~RenderWindow();

  // Delegate the actual window creation to concrete implementation
  // since they need to setup window hints for supported graphics api
  virtual void Create(
    i32 width, i32 height, const char *Title, WindowMode mode = WIN_MODE_WINDOWED
  ) = 0;

  virtual void Destroy() = 0;

  // TODO: Delegate this method to input manager class
  i32 GetKey(KeyCode key) const;

  // @brief poll events for input handling
  void PollEvents() const;

  /// @return true if window is closing
  b8 ShouldClose() const;

  /// @return the enum value of the current window mode
  WindowMode GetCurrentWindowMode() const;

  // TODO: Turn this to a flag when things get bigger
  void EnableVsync(i32 vsync);

  void SetWindowMode(WindowMode mode);

  void SetFullScreen(i32 fullscreen);

  void SetShouldClose(i32 value);

  void ToggleFullScreen();

  operator GLFWwindow *() const;

  GLFWwindow *GetHandle() { return m_Context; }

  virtual void MakeCurrent() override;

  virtual void SwapBuffers() override;

protected:
  GLFWwindow *m_Context;
  u32 m_PosX, m_PosY;
  WindowMode m_Mode;
};

#endif // !SN_WINDOW_CONTEXT_H
