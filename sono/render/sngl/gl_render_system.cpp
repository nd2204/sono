#include "gl_buffer_base.h"
#include "gl_render_device.h"
#include "gl_texture.h"
#include "gl_vertex_array.h"
#include "gl_window.h"
#include "render/shader/shader.h"
#include "gl_render_system.h"
#include "gl_commmon.h"

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
  m_pDevice = m_Arena.New<GLRenderDevice>(&m_Arena);
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Shutdown() {
  LOG_INFO("<-- Shutting down GLRenderSystem -->");
  m_pDevice->DeleteAllBuffers();
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
void GLRenderSystem::Draw(
  PrimitiveType topology, const VertexArray *va, u32 offset, u32 maxVertCount
) {
  const GLVertexArray *vao = reinterpret_cast<const GLVertexArray *>(va);
  i32 numBuffers = vao->GetVertexBuffers().size();
  va->Bind();
  for (int i = 0; i < numBuffers; i++) {
    glDrawArrays(ConvertPrimitiveType(topology), offset, maxVertCount);
    GL_CHECK_ERROR();
  }
}
// --------------------------------------------------------------------------------
void GLRenderSystem::DrawIndexed(
  PrimitiveType topology, const VertexArray *va, u32 idxOffset, u32 idxCount
) {
  const GLVertexArray *vao = reinterpret_cast<const GLVertexArray *>(va);
  const Buffer *ib = vao->GetCurrentIndexBuffer();

  GLenum type = GL_UNSIGNED_INT;
  if (ib->GetStride() == sizeof(u16))
    type = GL_UNSIGNED_SHORT;
  else if (ib->GetStride() == sizeof(u8))
    type = GL_UNSIGNED_BYTE;

  va->Bind();
  // clang-format off
  glDrawElements(
    ConvertPrimitiveType(topology),
    idxCount,
    type,
    reinterpret_cast<void *>(idxOffset)
  );
  // clang-format on
}
// --------------------------------------------------------------------------------
void GLRenderSystem::Clear(const Vec4 &c) {
  glClearColor(c.r, c.g, c.b, c.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL_CHECK_ERROR();
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
void GLRenderSystem::BindTexture(Texture *texture, u32 unit) {
  reinterpret_cast<GLTexture *>(texture)->Bind(unit);
  std::string uniformName = "uTexture" + std::to_string(unit);
  if (m_pActivePipeline) {
    m_pActivePipeline->Bind();
    m_pActivePipeline->SetUniform(uniformName.c_str(), (i32)unit);
  }
}
// --------------------------------------------------------------------------------
void GLRenderSystem::BindBuffer(Buffer *buffer, u32 index) {
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

  ImGuiStyle &style = ImGui::GetStyle();
  style.FrameRounding = 2.0f;
  style.FrameBorderSize = 1.0f;

  ImVec4 *colors = style.Colors;
  colors[ImGuiCol_Text] = ImVec4(0.92f, 0.86f, 0.70f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.22f, 0.21f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
  colors[ImGuiCol_Border] = ImVec4(0.31f, 0.29f, 0.27f, 0.50f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.19f, 0.18f, 0.40f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.22f, 0.21f, 0.67f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.72f, 0.73f, 0.15f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.29f, 0.27f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.54f, 0.66f, 0.54f, 0.40f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.23f, 0.23f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.23f, 0.23f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.31f, 0.29f, 0.27f, 0.50f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.73f, 0.15f, 0.78f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.72f, 0.73f, 0.15f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.22f, 0.21f, 0.20f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.22f, 0.21f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.72f, 0.73f, 0.15f, 0.95f);
  colors[ImGuiCol_InputTextCursor] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
  colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 0.86f);
  colors[ImGuiCol_TabSelected] = ImVec4(0.24f, 0.22f, 0.21f, 1.00f);
  colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_TabDimmed] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
  colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
  colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.72f, 0.73f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
  colors[ImGuiCol_TreeLines] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
  colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

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
    case PrimitiveType::POINTS:           return GL_POINTS;
    case PrimitiveType::LINES:            return GL_LINES;
    case PrimitiveType::LINE_LOOPS:       return GL_LINE_LOOP;
    case PrimitiveType::LINE_STRIPS:      return GL_LINE_STRIP;
    case PrimitiveType::TRIANGLES:        return GL_TRIANGLES;
    case PrimitiveType::TRIANGLE_STRIPS:  return GL_TRIANGLE_STRIP;
    case PrimitiveType::TRIANGLE_FAN:     return GL_TRIANGLE_FAN;
  }
  ASSERT(false);
  return 0;
}

// clang-format on
