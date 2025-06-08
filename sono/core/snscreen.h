#ifndef SN_WINDOW_H
#define SN_WINDOW_H

#include "common/types.h"

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <utility>

namespace Sono {

class SNScreen;

enum WindowMode {
  WINDOWMODE_WINDOWED,
  WINDOWMODE_FULLSCREEN,
  WINDOWMODE_BORDERLESS
};

class SNScreen {
public:
  using SNFrameBufferSizeCallback =
    std::function<void(SNScreen &win, i32 w, i32 h)>;

  SNScreen(
    i32 width,
    i32 height,
    const char *Title,
    WindowMode mode = WINDOWMODE_WINDOWED
  );

  u32 GetWidth() const;
  u32 GetHeight() const;
  i32 GetKey(i32 key) const;
  void PollEvents() const;
  i32 ShouldClose() const;
  const GLFWwindow *GetContext() const;
  WindowMode GetCurrentWindowMode() const;
  void SwapBuffers() const;

  // TODO: Turn this to a flag when things get bigger
  void EnableVsync(i32 vsync);
  void SetWindowMode(WindowMode mode);
  void SetFullScreen(i32 fullscreen);
  void SetShouldClose(i32 value);
  void ToggleFullScreen();
  void SetFrameBufferSizeCallback(SNFrameBufferSizeCallback cb);

  // Convert this screen object to GLFW window ptr
  operator GLFWwindow *() const;

  void OnFrameBufferResize(i32 width, i32 height);

private:
  struct CallbackState {
    SNScreen *thisPtr;
    SNFrameBufferSizeCallback cb;

    CallbackState(SNScreen *screen, SNFrameBufferSizeCallback cb)
      : thisPtr(screen)
      , cb(std::move(cb)) {}
  };

  GLFWwindow *m_Context;
  WindowMode m_Mode;
  u32 m_PosX, m_PosY;
  u32 m_Width, m_Height;
  std::shared_ptr<CallbackState> m_CallbackState;
};

} // namespace Sono

#endif // !SN_WINDOW_H