#include "global.h"
#include "debug/profiler.h"
#include "render/sngl/gl_render_system.h"
#include "core/common/time.h"
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

  // TODO: remove this
  m_Allocator.AllocateArena(1024);

  m_RenderSystem = std::make_unique<GLRenderSystem>();
  m_RenderSystem->Init();

  m_EventQueue = std::make_unique<SystemEventQueue>();

  m_InputSystem = std::make_unique<InputSystem>(&m_Allocator);
  m_InputSystem->Init();

#ifdef SN_DEBUG_PROFILER
  m_Profiler = std::make_unique<Profiler>();
  m_Profiler->AddSinks<Sono::ConsoleProfileSink>();
  m_Profiler->AddSinks<Sono::JsonTraceSink>("profile.json");
  m_Profiler->BeginSession();
  m_Profiler->Init();
#endif

  Time::Start();
}

//--------------------------------------------------------------------------------

// shutting down in the reverse order
void Global::Shutdown() {
  m_InputSystem->Shutdown();

#ifdef SN_DEBUG_PROFILER
  m_Profiler->EndSession();
  m_Profiler->Shutdown();
#endif

  m_RenderSystem->Shutdown();

  m_Allocator.FreeInternalBuffer();
  S_LOG(MemorySystem::GetPtr()->GetAllocsReport());
  S_LOG(MemorySystem::GetPtr()->GetLeaksReport());
  m_MemSys->Shutdown();
}
