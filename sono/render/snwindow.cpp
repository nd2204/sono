#include "snwindow.h"
#include "core/common/logger.h"

// --------------------------------------------------------------------------------
Window::Window(i32 width, i32 height, const char *title, WindowMode mode)
  : m_Mode(mode)
  , m_CallbackState(nullptr)
  , m_PosX(0)
  , m_PosY(0) {

  this->m_Context = glfwCreateWindow(width, height, title, NULL, NULL);
  this->m_Width = width;
  this->m_Height = height;

  if (!this->m_Context) {
    LOG_ERROR("Failed to create window");
    glfwTerminate();
    exit(-1);
  }

  SetFrameBufferSizeCallback(nullptr);

  glfwMakeContextCurrent(this->m_Context);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    LOG_ERROR("Failed to initialize GLAD");
    exit(-1);
  }

  // TODO: remove condition after handled switching to windowed mode
  if (mode != WIN_MODE_WINDOWED) {
    SetWindowMode(mode);
  }
}
// --------------------------------------------------------------------------------
void Window::SetWindowMode(WindowMode mode) {
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
void Window::SetFullScreen(i32 fullscreen) {
  if (fullscreen) {
    SetWindowMode(WIN_MODE_FULLSCREEN);
  } else {
    SetWindowMode(WIN_MODE_WINDOWED);
  }
}
// --------------------------------------------------------------------------------
void Window::SetFrameBufferSizeCallback(SNFrameBufferSizeCallback cb) {
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
void Window::OnFrameBufferResize(i32 width, i32 height) {
  this->m_Width = width;
  this->m_Height = height;
}
// --------------------------------------------------------------------------------
WindowMode Window::GetCurrentWindowMode() const { return this->m_Mode; }
// --------------------------------------------------------------------------------
i32 Window::GetKey(i32 key) const { return glfwGetKey(this->m_Context, key); }
// --------------------------------------------------------------------------------
void Window::EnableVsync(i32 vsync) { glfwSwapInterval(vsync); }
// --------------------------------------------------------------------------------
void Window::SetShouldClose(i32 value) { glfwSetWindowShouldClose(this->m_Context, value); }
// --------------------------------------------------------------------------------
void Window::PollEvents() const { glfwPollEvents(); }
// --------------------------------------------------------------------------------
b8 Window::ShouldClose() const { return glfwWindowShouldClose(this->m_Context); };
// --------------------------------------------------------------------------------
void Window::SwapBuffers() { glfwSwapBuffers(this->m_Context); }
// --------------------------------------------------------------------------------
Window::operator GLFWwindow *() const { return this->m_Context; }
