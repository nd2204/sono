#include "core/input/key_code.h"

#include "render/render_window.h"
#include <GLFW/glfw3.h>

// --------------------------------------------------------------------------------
RenderWindow::RenderWindow()
  : RenderContext(0, 0)
  , m_PosX(0)
  , m_PosY(0)
  , m_Mode(WIN_MODE_WINDOWED)
  , m_CurrentCursorMode(CursorMode::NORMAL) {}
// --------------------------------------------------------------------------------
RenderWindow::~RenderWindow() {}
// --------------------------------------------------------------------------------
void RenderWindow::SetWindowMode(WindowMode mode) {
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
  switch (mode) {
    case WIN_MODE_FULLSCREEN: {
      // Change position base on aspect ratio
      glfwSetWindowMonitor(
        this->m_Context, monitor, 20, 20, vidmode->width, vidmode->height, vidmode->refreshRate
      );
      break;
    }
    case WIN_MODE_BORDERLESS:
      glfwSetWindowMonitor(
        this->m_Context, monitor, 20, 20, vidmode->width, vidmode->height, vidmode->refreshRate
      );
      break;
    default:
      glfwSetWindowMonitor(
        this->m_Context, NULL, this->m_PosX, this->m_PosY, this->m_Width, this->m_Height,
        vidmode->refreshRate
      );
      break;
  };

  this->m_Mode = mode;
}
// --------------------------------------------------------------------------------
void RenderWindow::SetFullScreen(b8 fullscreen) {
  if (fullscreen) {
    SetWindowMode(WIN_MODE_FULLSCREEN);
  } else {
    SetWindowMode(WIN_MODE_WINDOWED);
  }
}
// --------------------------------------------------------------------------------
void RenderWindow::SetCursorMode(CursorMode mode) {
  m_CurrentCursorMode = mode;
  glfwSetInputMode(m_Context, GLFW_CURSOR, static_cast<i32>(mode));
}
// --------------------------------------------------------------------------------
CursorMode RenderWindow::GetCurrentCursorMode() const { return m_CurrentCursorMode; }
// --------------------------------------------------------------------------------
WindowMode RenderWindow::GetCurrentWindowMode() const { return this->m_Mode; }
// --------------------------------------------------------------------------------
i32 RenderWindow::GetKey(KeyCode key) const { return glfwGetKey(this->m_Context, (i32)key); }
// --------------------------------------------------------------------------------
void RenderWindow::EnableVsync(b8 vsync) { glfwSwapInterval(vsync); }
// --------------------------------------------------------------------------------
void RenderWindow::SetShouldClose(b8 value) { glfwSetWindowShouldClose(this->m_Context, value); }
// --------------------------------------------------------------------------------
void RenderWindow::PollEvents() const { glfwPollEvents(); }
// --------------------------------------------------------------------------------
b8 RenderWindow::ShouldClose() const { return glfwWindowShouldClose(this->m_Context); };
// --------------------------------------------------------------------------------
void RenderWindow::MakeCurrent() { glfwMakeContextCurrent(this->m_Context); }
// --------------------------------------------------------------------------------
void RenderWindow::SwapBuffers() { glfwSwapBuffers(this->m_Context); }
