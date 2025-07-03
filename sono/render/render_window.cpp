#include "render/render_window.h"
#include "GLFW/glfw3.h"
#include "core/common/logger.h"
#include "core/common/snassert.h"

// --------------------------------------------------------------------------------
RenderWindow::RenderWindow()
  : RenderContext(0, 0)
  , m_Mode(WIN_MODE_WINDOWED)
  , m_PosX(0)
  , m_PosY(0)
  , m_CallbackState(nullptr) {}
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
void RenderWindow::SetFullScreen(i32 fullscreen) {
  if (fullscreen) {
    SetWindowMode(WIN_MODE_FULLSCREEN);
  } else {
    SetWindowMode(WIN_MODE_WINDOWED);
  }
}
// --------------------------------------------------------------------------------
void RenderWindow::SetFrameBufferSizeCallback(SNFrameBufferSizeCallback cb) {
  m_CallbackState = std::make_shared<CallbackState>(this, std::move(cb));

  glfwSetWindowUserPointer(this->m_Context, m_CallbackState.get());
  glfwSetFramebufferSizeCallback(this->m_Context, [](GLFWwindow *window, int width, int height) {
    CallbackState *win = static_cast<CallbackState *>(glfwGetWindowUserPointer(window));

    win->thisPtr->OnFrameBufferResize(width, height);
    if (win->cb) {
      win->cb(*(win->thisPtr), width, height);
    }
  });
}
// --------------------------------------------------------------------------------
void RenderWindow::OnFrameBufferResize(i32 width, i32 height) {
  this->m_Width = width;
  this->m_Height = height;
}
// --------------------------------------------------------------------------------
WindowMode RenderWindow::GetCurrentWindowMode() const { return this->m_Mode; }
// --------------------------------------------------------------------------------
i32 RenderWindow::GetKey(i32 key) const { return glfwGetKey(this->m_Context, key); }
// --------------------------------------------------------------------------------
void RenderWindow::EnableVsync(i32 vsync) { glfwSwapInterval(vsync); }
// --------------------------------------------------------------------------------
void RenderWindow::SetShouldClose(i32 value) { glfwSetWindowShouldClose(this->m_Context, value); }
// --------------------------------------------------------------------------------
void RenderWindow::PollEvents() const { glfwPollEvents(); }
// --------------------------------------------------------------------------------
b8 RenderWindow::ShouldClose() const { return glfwWindowShouldClose(this->m_Context); };
// --------------------------------------------------------------------------------
void RenderWindow::MakeCurrent() { glfwMakeContextCurrent(this->m_Context); }
// --------------------------------------------------------------------------------
void RenderWindow::SwapBuffers() { glfwSwapBuffers(this->m_Context); }
// --------------------------------------------------------------------------------
RenderWindow::operator GLFWwindow *() const { return this->m_Context; }
