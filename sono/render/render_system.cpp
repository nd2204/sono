#include "core/common/logger.h"
#include "render_system.h"
#include <sstream>

template <>
RenderSystem *Singleton<RenderSystem>::m_sInstance = nullptr;

RenderSystem::RenderSystem()
  : m_pRenderCtx(nullptr) {}

Window *RenderSystem::CreateRenderWindow(
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

RenderSystem::~RenderSystem() {}
