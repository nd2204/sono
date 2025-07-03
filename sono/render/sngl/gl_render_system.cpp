#include "gl_buffer_base.h"
#include "gl_buffer_manager.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_vertex_array.h"
#include "gl_window.h"
#include "gl_render_system.h"
#include "core/memory/memory_system.h"
#include <memory>
#include <string>

GLRenderSystem::GLRenderSystem()
  : RenderSystem() {}

void GLRenderSystem::Init() {
  LOG_INFO("<-- Initializing GLRenderSystem -->");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  m_pBufferManager = m_Arena.Create<GLBufferManager>();
}

void GLRenderSystem::Shutdown() {
  LOG_INFO("<-- Shutting down GLRenderSystem -->");
  m_Arena.Clear();
}

VertexArray *GLRenderSystem::CreateVertexArray() { return m_Arena.Create<GLVertexArray>(); }

void GLRenderSystem::BindVertexArray(VertexArray *va) {
  return reinterpret_cast<GLVertexArray *>(va)->Bind();
}

void GLRenderSystem::SetRenderContext(RenderContext *ctx) { m_pActiveCtx = ctx; }

void GLRenderSystem::BeginFrame() { m_FrameBeginMark = m_Arena.GetMarker(); }

void GLRenderSystem::EndFrame() { m_Arena.FreeToMarker(m_FrameBeginMark); }

void GLRenderSystem::DrawIndexed(PrimitiveType topology, VertexArray *va, u32 idxCount) {
  reinterpret_cast<GLVertexArray *>(va)->DrawIndexed(ConvertPrimitiveType(topology));
}

void GLRenderSystem::Clear(const Vec4 &color) {
  Vec4 c = color.Normalized();
  glClearColor(c.r, c.g, c.b, c.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderSystem::Present() { m_pActiveCtx->SwapBuffers(); }

Shader *GLRenderSystem::CreateShader(const ShaderDesc *desc) {
  return m_Arena.Create<GLShader>(desc);
}

Texture *GLRenderSystem::CreateTexture(
  TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
) {
  return m_Arena.Create<GLTexture>(type, internalFmt, fmt, width, height);
}

RenderPipeline *GLRenderSystem::CreatePipeline(Shader *vs, Shader *fs) {
  return m_Arena.Create<GLRenderPipeline>(vs, fs);
}

void GLRenderSystem::BindTexture(Texture *texture, u32 unit) {
  reinterpret_cast<GLTexture *>(texture)->Bind(unit);
  std::string uniformName = "uTexture" + std::to_string(unit);
  if (m_pActivePipeline) {
    GLRenderPipeline *pipeline = reinterpret_cast<GLRenderPipeline *>(m_pActivePipeline);
    pipeline->Bind();
    pipeline->SetInt(uniformName.c_str(), unit);
  }
}

void GLRenderSystem::BindBuffer(IBuffer *buffer, u32 index) {
  reinterpret_cast<GLBuffer *>(buffer)->Bind();
}

void GLRenderSystem::BindPipeline(RenderPipeline *pipeline, u32 index) {
  m_pActivePipeline = pipeline;
  (void)index;
  reinterpret_cast<GLRenderPipeline *>(pipeline)->Bind();
}

void GLRenderSystem::UnbindPipeline(RenderPipeline *pipeline, u32 index) {
  (void)index;
  reinterpret_cast<GLRenderPipeline *>(pipeline)->Unbind();
}

RenderWindow *GLRenderSystem::CreateRenderWindow(
  i32 width, i32 height, const char *title, WindowMode mode
) {
  RenderSystem::CreateRenderWindow(width, height, title, mode);
  GLWindow *window = m_Arena.Create<GLWindow>();
  window->Create(width, height, title, mode);
  window->MakeCurrent();
  m_pActiveCtx = window;
  return window;
}

// clang-format off

GLenum GLRenderSystem::ConvertPrimitiveType(PrimitiveType type) {
  switch (type) {
    case PrimitiveType::TRIANGLES:         return GL_TRIANGLES;
    case PrimitiveType::TRIANGLE_STRIPS:   return GL_TRIANGLE_STRIP;
    case PrimitiveType::TRIANGLE_FAN:      return GL_TRIANGLE_FAN;
    case PrimitiveType::LINES:             return GL_LINE;
    case PrimitiveType::LINE_STRIPS:       return GL_LINE_STRIP;
    case PrimitiveType::POINTS:            return GL_POINT;
  }
  ASSERT(false);
  return 0;
}

// clang-format on
