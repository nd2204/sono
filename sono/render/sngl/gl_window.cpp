#include "core/common/snassert.h"

#include "gl_window.h"

#include <GLFW/glfw3.h>
#include <cstdlib>

GLWindow::GLWindow() {}

GLWindow::~GLWindow() { Destroy(); }

void GLWindow::Create(i32 width, i32 height, const char *title, WindowMode mode) {
  this->m_Context = glfwCreateWindow(width, height, title, NULL, NULL);
  SN_ASSERT(this->m_Context, "Failed to create window");

  this->m_Width = width;
  this->m_Height = height;

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

void GLWindow::Destroy() {
  if (this->m_Context) {
    this->m_Context = nullptr;
    glfwDestroyWindow(this->m_Context);
  }
}
