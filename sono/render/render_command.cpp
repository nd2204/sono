#include "render_command.h"
#include "render_system.h"

DrawCommand::DrawCommand(VertexArray *va, u32 vertCount, Texture *tex, PrimitiveType topology)
  : m_VAO(va)
  , m_Tex(tex)
  , m_NumVertices(vertCount)
  , m_Topology(topology) {}

void DrawCommand::Execute(RenderSystem &renderSys) const {
  if (m_Tex) {
    renderSys.BindTexture(m_Tex, 0);
  }
  renderSys.Draw(m_Topology, m_VAO, m_NumVertices);
}

DrawIndexedCommand::DrawIndexedCommand(
  VertexArray *va, u32 idxCount, Texture *tex, PrimitiveType topology
)
  : m_VAO(va)
  , m_Tex(tex)
  , m_NumIndex(idxCount)
  , m_Topology(topology) {}

void DrawIndexedCommand::Execute(RenderSystem &renderSys) const {
  if (m_Tex) {
    renderSys.BindTexture(m_Tex, 0);
  }
  renderSys.DrawIndexed(m_Topology, m_VAO, m_NumIndex);
}

ClearCommand::ClearCommand(const Vec4 &color)
  : m_Color(color) {}

void ClearCommand::Execute(RenderSystem &renderSys) const { renderSys.Clear(m_Color); }
