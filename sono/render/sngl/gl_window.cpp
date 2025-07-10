#include "core/common/snassert.h"
#include "core/common/sys_events.h"
#include "core/common/sys_event_queue.h"
#include "core/common/time.h"

#include <glad/glad.h>
#include "gl_window.h"
#include "core/debug/profiler.h"

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

  glEnable(GL_DEPTH_TEST);

  // TODO: remove condition after handled switching to windowed mode
  if (mode != WIN_MODE_WINDOWED) {
    SetWindowMode(mode);
  }

  glfwSetInputMode(m_Context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetFramebufferSizeCallback(m_Context, [](GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
    Event e;
    e.time = Time::Now() * 1000;
    e.type = EventType::WINDOW_RESIZE;
    e.payload = WindowResizeEvent{width, height};
    SN_QUEUE_EVENT(e)
  });

  glfwSetCursorPosCallback(m_Context, [](GLFWwindow *window, double xpos, double ypos) {
    (void)window;
    Event e;
    e.time = Time::Now() * 1000;
    e.type = EventType::MOUSE_MOVE;
    e.payload = MouseMoveEvent{xpos, ypos};
    SN_QUEUE_EVENT(e)
  });

  glfwSetMouseButtonCallback(m_Context, [](GLFWwindow *window, int button, int action, int mods) {
    (void)window;
    Event e;
    e.time = Time::Now() * 1000;
    e.type = EventType::MOUSE_BUTTON;
    e.payload = MouseButtonEvent{button, (b8)action};
    SN_QUEUE_EVENT(e)
  });
}

void GLWindow::Destroy() {
  if (this->m_Context) {
    this->m_Context = nullptr;
    glfwDestroyWindow(this->m_Context);
  }
}
