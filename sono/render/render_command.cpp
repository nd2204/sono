#include "render_command.h"
#include "render_system.h"
#include "shader/shader.h"

BindShaderCommand::BindShaderCommand(RenderPipeline *shader)
  : m_Pipeline(shader) {}

void BindShaderCommand::Execute(RenderSystem &renderSys) const {
  renderSys.BindPipeline(m_Pipeline, 0);
}

// --------------------------------------------------------------------------------

template class SetUniformCommand<Mat4>;
template class SetUniformCommand<Mat3>;
template class SetUniformCommand<Vec4>;
template class SetUniformCommand<Vec3>;
template class SetUniformCommand<Vec2>;
template class SetUniformCommand<Color3>;
template class SetUniformCommand<Color>;
template class SetUniformCommand<f32>;
template class SetUniformCommand<i32>;

template <typename T>
void SetUniformCommand<T>::Execute(RenderSystem &renderSys) const {
  renderSys.GetCurrentPipeline()->SetUniform(m_Uniform, m_Data);
}

// --------------------------------------------------------------------------------
SetViewportCommand::SetViewportCommand(i32 xpos, i32 ypos, i32 width, i32 height)
  : m_PosX(xpos)
  , m_PosY(ypos)
  , m_Width(width)
  , m_Height(height) {}
// --------------------------------------------------------------------------------
void SetViewportCommand::Execute(RenderSystem &renderSys) const {
  renderSys.SetViewport(m_PosX, m_PosY, m_Width, m_Height);
}
// --------------------------------------------------------------------------------
DrawCommand::DrawCommand(
  const VertexArray *va, u32 vertCount, const Mat4 &model, PrimitiveType topology
)
  : m_VAO(va)
  , m_Transform(model)
  , m_NumVertices(vertCount)
  , m_Topology(topology) {}
// --------------------------------------------------------------------------------
void DrawCommand::Execute(RenderSystem &renderSys) const {
  renderSys.GetCurrentPipeline()->SetUniform("uModel", m_Transform);
  renderSys.Draw(m_Topology, m_VAO, m_NumVertices);
}
// --------------------------------------------------------------------------------
DrawIndexedCommand::DrawIndexedCommand(
  const VertexArray *va, u32 idxCount, const Mat4 &model, PrimitiveType topology
)
  : m_VAO(va)
  , m_Transform(model)
  , m_NumIndex(idxCount)
  , m_Topology(topology) {}
// --------------------------------------------------------------------------------
void DrawIndexedCommand::Execute(RenderSystem &renderSys) const {
  renderSys.GetCurrentPipeline()->SetUniform("uModel", m_Transform);
  renderSys.DrawIndexed(m_Topology, m_VAO, m_NumIndex);
}
// --------------------------------------------------------------------------------
ClearCommand::ClearCommand(const Vec4 &color)
  : m_Color(color) {}
// --------------------------------------------------------------------------------
void ClearCommand::Execute(RenderSystem &renderSys) const { renderSys.Clear(m_Color); }
// --------------------------------------------------------------------------------
