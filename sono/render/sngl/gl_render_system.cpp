#include "gl_render_system.h"
#include "core/memory/memory_system.h"

void GLRenderSystem::Init() {
  LOG_INFO("<-- Initializing GLRenderSystem -->");
  /* Initialize the library */
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void GLRenderSystem::Shutdown() {
  LOG_INFO("<-- Shutting down GLRenderSystem -->");
  glfwTerminate();
}

Window *GLRenderSystem::CreateRenderWindow(
  i32 width, i32 height, const char *title, WindowMode mode
) {
  RenderSystem::CreateRenderWindow(width, height, title, mode);
  return SN_NEW(SN_ALLOCATION_TYPE_RENDER_SYSTEM) Window(width, height, title, mode);
}

void GLRenderSystem::SetRenderContext(RenderContext *ctx) { m_pRenderCtx = ctx; }

void GLRenderSystem::BeginFrame() {
  SN_ASSERT(false, "GLRenderSystem::BeginFrame not implemented");
}

void GLRenderSystem::Render(const RenderOp &op) {
  SN_ASSERT(false, "GLRenderSystem::Render not implemented");
}

void GLRenderSystem::EndFrame() { SN_ASSERT(false, "GLRenderSystem::EndFrame not implemented"); }
