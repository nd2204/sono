#include "snscreen.h"
#include "common/logger.h"

#include "GLFW/glfw3.h"
#include <memory>
#include <utility>
#include <windows.h>

using namespace Sono;

SNScreen::SNScreen(i32 width, i32 height, const char *title, WindowMode mode)
  : m_Mode(mode)
  , m_PosX(0)
  , m_PosY(0)
  , m_Width(width)
  , m_Height(height)
  , m_CallbackState(nullptr) {
  this->m_Context = glfwCreateWindow(width, height, title, NULL, NULL);

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

  //
  // TODO: remove condition after handled switching to windowed mode
  if (mode != WINDOWMODE_WINDOWED) {
    SetWindowMode(mode);
  }
}

void SNScreen::SetWindowMode(WindowMode mode) {
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
  switch (mode) {
  case WINDOWMODE_FULLSCREEN: {
    // Change position base on aspect ratio
    glfwSetWindowMonitor(
      this->m_Context,
      monitor,
      20,
      20,
      vidmode->width,
      vidmode->height,
      vidmode->refreshRate
    );
    break;
  }
  case WINDOWMODE_BORDERLESS:
    glfwSetWindowMonitor(
      this->m_Context,
      monitor,
      20,
      20,
      vidmode->width,
      vidmode->height,
      vidmode->refreshRate
    );
    break;
  default:
    glfwSetWindowMonitor(
      this->m_Context,
      NULL,
      this->m_PosX,
      this->m_PosY,
      this->m_Width,
      this->m_Height,
      vidmode->refreshRate
    );
    break;
  };

  this->m_Mode = mode;
}

void SNScreen::SetFullScreen(i32 fullscreen) {
  if (fullscreen) {
    SetWindowMode(WINDOWMODE_FULLSCREEN);
  } else {
    SetWindowMode(WINDOWMODE_WINDOWED);
  }
}

void SNScreen::SetFrameBufferSizeCallback(SNFrameBufferSizeCallback cb) {
  m_CallbackState = std::make_shared<CallbackState>(this, std::move(cb));

  glfwSetWindowUserPointer(this->m_Context, m_CallbackState.get());
  glfwSetFramebufferSizeCallback(
    this->m_Context,
    [](GLFWwindow *window, int width, int height) {
      CallbackState *win =
        static_cast<CallbackState *>(glfwGetWindowUserPointer(window));

      win->thisPtr->OnFrameBufferResize(width, height);
      if (win->cb) {
        win->cb(*(win->thisPtr), width, height);
      }
    }
  );
}

u32 SNScreen::GetWidth() const { return this->m_Width; }

u32 SNScreen::GetHeight() const { return this->m_Height; }

i32 SNScreen::GetKey(i32 key) const { return glfwGetKey(this->m_Context, key); }

const GLFWwindow *SNScreen::GetContext() const { return this->m_Context; }

void SNScreen::OnFrameBufferResize(i32 width, i32 height) {
  this->m_Width = width;
  this->m_Height = height;
}

WindowMode SNScreen::GetCurrentWindowMode() const { return this->m_Mode; }

void SNScreen::EnableVsync(i32 vsync) { glfwSwapInterval(vsync); }

void SNScreen::SetShouldClose(i32 value) {
  glfwSetWindowShouldClose(this->m_Context, value);
}

void SNScreen::PollEvents() const { glfwPollEvents(); }

i32 SNScreen::ShouldClose() const {
  return glfwWindowShouldClose(this->m_Context);
};

void SNScreen::SwapBuffers() const { glfwSwapBuffers(this->m_Context); }

SNScreen::operator GLFWwindow *() const { return this->m_Context; }