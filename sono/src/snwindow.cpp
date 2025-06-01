#include "sono/snwindow.h"
#include <iostream>

SNWindow::SNWindow(i32 width, i32 height, const char *title, WindowMode mode)
  : m_Mode(mode)
  , m_PosX(0)
  , m_PosY(0)
  , m_Width(width)
  , m_Height(height) {
  this->m_Context = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!this->m_Context) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(this->m_Context);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(EXIT_FAILURE);
  }

  //
  // TODO: remove condition after handled switching to windowed mode
  if (mode != WINDOWMODE_WINDOWED) {
    SetWindowMode(mode);
  }
}

void SNWindow::SetWindowMode(WindowMode mode) {
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

void SNWindow::SetFullScreen(i32 fullscreen) {
  if (fullscreen) {
    SetWindowMode(WINDOWMODE_FULLSCREEN);
  } else {
    SetWindowMode(WINDOWMODE_WINDOWED);
  }
}

u32 SNWindow::GetWidth() { return this->m_Width; }

u32 SNWindow::GetHeight() { return this->m_Height; }

WindowMode SNWindow::GetCurrentWindowMode() { return this->m_Mode; }

void SNWindow::EnableVsync(i32 vsync) { glfwSwapInterval(vsync); }

SNWindow::operator GLFWwindow *() const { return this->m_Context; }