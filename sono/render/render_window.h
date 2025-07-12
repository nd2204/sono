#ifndef SN_WINDOW_CONTEXT_H
#define SN_WINDOW_CONTEXT_H

#include "GLFW/glfw3.h"
#include "render_context.h"

#include <memory>
#include <utility>

#include <functional>
#include <core/input/key_code.h>

enum WindowMode { WIN_MODE_WINDOWED, WIN_MODE_FULLSCREEN, WIN_MODE_BORDERLESS };

enum class CursorMode : i32 {
  CAPTURED = GLFW_CURSOR_CAPTURED,
  NORMAL = GLFW_CURSOR_NORMAL,
  DISABLED = GLFW_CURSOR_DISABLED
};

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

  void EnableVsync(b8 vsync);

  void ToggleFullScreen();

  // ================================================================================
  // Getter & Setter
  // ================================================================================

  void SetWindowMode(WindowMode mode);

  void SetFullScreen(b8 fullscreen);

  /// @brief set the should close state
  void SetShouldClose(b8 value);

  /// @brief set cursor mode
  void SetCursorMode(CursorMode mode);

  /// @return the internal handler of glfw window context
  GLFWwindow *GetHandle() { return m_Context; }

  /// @return the enum value of the current window mode
  WindowMode GetCurrentWindowMode() const;

  /// @return the enum value of the current cursor mode
  CursorMode GetCurrentCursorMode() const;

  // ================================================================================
  // Overloaded function
  // ================================================================================

  void MakeCurrent() override;

  void SwapBuffers() override;

protected:
  GLFWwindow *m_Context;
  u32 m_PosX, m_PosY;
  WindowMode m_Mode;

  CursorMode m_CurrentCursorMode;
};

#endif // !SN_WINDOW_CONTEXT_H
