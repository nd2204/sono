#include "gl_buffer_base.h"
#include "gl_buffer_manager.h"
#include "gl_shader.h"
#include "gl_texture.h"
#include "gl_vertex_array.h"
#include "gl_window.h"
#include "gl_render_system.h"

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <string>

GLRenderSystem::GLRenderSystem()
  : RenderSystem() {}
// --------------------------------------------------------------------------------
void GLRenderSystem::Init() {
  LOG_INFO("<-- Initializing GLRenderSystem -->");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef MACOS_BUILD
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  m_pBufferManager = m_Arena.New<GLBufferManager>();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Shutdown() {
  LOG_INFO("<-- Shutting down GLRenderSystem -->");
  m_pBufferManager->DeleteAllIndexBuffers();
  m_pBufferManager->DeleteAllVertexBuffers();
  m_pBufferManager->DeleteAllLayout();
  m_Arena.FreeInternalBuffer();
}
// --------------------------------------------------------------------------------
RenderWindow *GLRenderSystem::CreateRenderWindow(
  i32 width, i32 height, const char *title, WindowMode mode
) {
  RenderSystem::CreateRenderWindow(width, height, title, mode);
  GLWindow *window = m_Arena.New<GLWindow>();
  window->Create(width, height, title, mode);
  window->MakeCurrent();
  m_pActiveCtx = window;
  return window;
}

// ================================================================================
// Render operation
// ================================================================================

void GLRenderSystem::BeginFrame(const Camera &cam) {
  m_FrameBeginMark = m_Arena.GetMarker();

  if (m_pActivePipeline) {
    m_pActivePipeline->SetUniform("uView", cam.GetViewMatrix());
    m_pActivePipeline->SetUniform("uProj", cam.GetProjectionMatrix());
  }
}
// --------------------------------------------------------------------------------
void GLRenderSystem::EndFrame() {
  m_Arena.FreeToMarker(m_FrameBeginMark);
  /* Swap front and back buffers */
  Flush();
  Present();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Present() { m_pActiveCtx->SwapBuffers(); }
// --------------------------------------------------------------------------------
void GLRenderSystem::SetViewport(i32 posX, i32 posY, i32 width, i32 height) {
  glViewport(posX, posY, width, height);
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Draw(PrimitiveType topology, const VertexArray *va, u32 maxVertCount) {
  const GLVertexArray *vao = reinterpret_cast<const GLVertexArray *>(va);
  i32 numBuffers = vao->GetVertexBuffers().size();
  va->Bind();
  for (int i = 0; i < numBuffers; i++) {
    glDrawArrays(ConvertPrimitiveType(topology), 0, maxVertCount);
  }
}
// --------------------------------------------------------------------------------
void GLRenderSystem::DrawIndexed(PrimitiveType topology, const VertexArray *va, u32 idxCount) {
  const GLVertexArray *vao = reinterpret_cast<const GLVertexArray *>(va);
  const GLIndexBuffer *ib = vao->GetCurrentIndexBuffer();
  va->Bind();
  glDrawElements(
    ConvertPrimitiveType(topology), idxCount, GLIndexBuffer::ConvertIndexType(ib->GetIndexType()),
    reinterpret_cast<void *>(0)
  );
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Clear(const Vec4 &c) {
  glClearColor(c.r, c.g, c.b, c.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ================================================================================
// Pipeline management
// ================================================================================

VertexArray *GLRenderSystem::CreateVertexArray() { return m_Arena.New<GLVertexArray>(); }
// --------------------------------------------------------------------------------
void GLRenderSystem::BindVertexArray(VertexArray *va) {
  return reinterpret_cast<GLVertexArray *>(va)->Bind();
}
// --------------------------------------------------------------------------------
Shader *GLRenderSystem::CreateShader(const ShaderDesc *desc) { return m_Arena.New<GLShader>(desc); }
// --------------------------------------------------------------------------------
Texture *GLRenderSystem::CreateTexture(
  TextureType type, TextureFormat internalFmt, TextureFormat fmt, u32 width, u32 height
) {
  return m_Arena.New<GLTexture>(type, internalFmt, fmt, width, height);
}
// --------------------------------------------------------------------------------
RenderPipeline *GLRenderSystem::CreatePipeline(Shader *vs, Shader *fs) {
  return m_Arena.New<GLRenderPipeline>(vs, fs);
}
// --------------------------------------------------------------------------------
void GLRenderSystem::BindTexture(Texture *texture, u32 unit) {
  reinterpret_cast<GLTexture *>(texture)->Bind(unit);
  std::string uniformName = "uTexture" + std::to_string(unit);
  if (m_pActivePipeline) {
    m_pActivePipeline->Bind();
    m_pActivePipeline->SetUniform(uniformName.c_str(), (i32)unit);
  }
}
// --------------------------------------------------------------------------------
void GLRenderSystem::BindBuffer(IBuffer *buffer, u32 index) {
  (void)index;
  buffer->Bind();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::BindPipeline(RenderPipeline *pipeline, u32 index) {
  (void)index;
  m_pActivePipeline = pipeline;
  pipeline->Bind();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::UnbindPipeline(RenderPipeline *pipeline, u32 index) {
  (void)index;
  pipeline->Unbind();
}

// ================================================================================
// ImGui
// ================================================================================

void GLRenderSystem::InitImGui(RenderWindow *window) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}
// --------------------------------------------------------------------------------
void GLRenderSystem::ShutdownImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::BeginImGuiFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}
// --------------------------------------------------------------------------------
void GLRenderSystem::EndImGuiFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ================================================================================
// Getters & Setters
// ================================================================================

void GLRenderSystem::SetRenderContext(RenderContext *ctx) { m_pActiveCtx = ctx; }

// --------------------------------------------------------------------------------
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
