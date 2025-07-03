#include "buffer_manager.h"
#include "core/common/logger.h"
#include "core/memory/memory_system.h"
#include "render_system.h"

#include <sstream>
#include <GLFW/glfw3.h>

#define RENDER_FRAME_ALLOC_SIZE 256 * SN_MEM_MIB

template <>
RenderSystem *Singleton<RenderSystem>::m_sInstance = nullptr;

RenderSystem::RenderSystem()
  : m_pActiveCtx(nullptr)
  , m_pBufferManager(nullptr)
  , m_pActivePipeline(nullptr)
  , m_Arena(RENDER_FRAME_ALLOC_SIZE) {
  /* Initialize the library */
  if (!glfwInit()) exit(EXIT_FAILURE);
}

RenderWindow *RenderSystem::CreateRenderWindow(
  i32 width, i32 height, const char *title, WindowMode mode
) {
  std::stringstream ss;

  ss << "Created render context (" << title << "," << width << "x" << height << ",";
  switch (mode) {
  case WIN_MODE_WINDOWED:
    ss << "windowed";
    break;
  case WIN_MODE_FULLSCREEN:
    ss << "fullscreen";
    break;
  case WIN_MODE_BORDERLESS:
    ss << "borderless";
    break;
  }
  ss << ")";
  LOG_DEBUG(ss.str().c_str());

  return nullptr;
}

void RenderSystem::Flush() { m_RenderQueue.Flush(*this); }

BufferManager *RenderSystem::GetBufferManager() const { return m_pBufferManager; }

RenderSystem::~RenderSystem() { glfwTerminate(); }
