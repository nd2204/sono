#include "render/render_system.h"
#include "debug_draw.h"
#include "render/vertex_type.h"

DebugDraw::DebugDraw(RenderSystem *render)
  : m_RenderSys(render)
  , m_Buffer(nullptr)
  , m_VertexArray(nullptr) {}
// --------------------------------------------------------------------------------
void DebugDraw::Init() {
  RenderDevice *device = m_RenderSys->GetRenderDevice();
  m_Buffer = device->CreateBuffer(
    {.count = 10000,
     .stride = sizeof(VertexPC),
     .usage = BufferUsage::Vertex | BufferUsage::MapWrite}
  );
  m_VertexArray = device->CreateVertexArray();
  m_VertexArray->AddVertexBuffer(m_Buffer, VertexTraits<VertexPC>::GetLayout());
}
// --------------------------------------------------------------------------------
void DebugDraw::Shutdown() {
  RenderDevice *device = m_RenderSys->GetRenderDevice();
  device->DeleteBuffer(m_Buffer);
}
// --------------------------------------------------------------------------------
