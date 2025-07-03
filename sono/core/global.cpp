#include "global.h"
#include "render/sngl/gl_render_system.h"
#include <memory>

using namespace Sono;

template <>
Global *Singleton<Global>::m_sInstance = nullptr;

//--------------------------------------------------------------------------------
Global::Global() {}

//--------------------------------------------------------------------------------
Global::~Global() {}

//--------------------------------------------------------------------------------
void Global::Init() {
  m_MemSys = std::make_unique<MemorySystem>();
  m_MemSys->Init();
  m_RenderSystem = std::make_unique<GLRenderSystem>();
  m_RenderSystem->Init();
}

//--------------------------------------------------------------------------------

// shutting down in the reverse order
void Global::Shutdown() {
  m_RenderSystem->Shutdown();

  S_LOG(MemorySystem::GetPtr()->GetAllocsReport());
  S_LOG(MemorySystem::GetPtr()->GetLeaksReport());
  m_MemSys->Shutdown();
}
