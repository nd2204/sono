#include <core/common/logger.h>
#include <core/memory/memory_system.h>
#include <core/debug/profiler.h>
#include <render/render_system.h>

#include <sstream>
#include <GLFW/glfw3.h>

#define RENDER_FRAME_ALLOC_SIZE (1 * SN_MEM_MIB)

RenderSystem::RenderSystem()
  : m_pActiveCtx(nullptr)
  , m_pDevice(nullptr)
  , m_pActivePipeline(nullptr)
  , m_Arena(RENDER_FRAME_ALLOC_SIZE) {
  /* Initialize the library */
  if (!glfwInit()) exit(EXIT_FAILURE);
  m_DebugDraw = m_Arena.New<DebugDraw>(this);
}

RenderWindow *RenderSystem::CreateRenderWindow(
  i32 width, i32 height, const char *title, WindowMode mode
) {
  std::ostringstream oss;

  oss << "Created render context (" << title << "," << width << "x" << height << ",";
  switch (mode) {
    case WIN_MODE_WINDOWED:
      oss << "windowed";
      break;
    case WIN_MODE_FULLSCREEN:
      oss << "fullscreen";
      break;
    case WIN_MODE_BORDERLESS:
      oss << "borderless";
      break;
  }
  oss << ")";
  LOG_DEBUG(oss.str().c_str());

  return nullptr;
}

void RenderSystem::Flush() {
  PROFILE_SCOPE("RenderSystem::Flush");
  m_RenderQueue.Flush(*this);
}

RenderSystem::~RenderSystem() { glfwTerminate(); }
