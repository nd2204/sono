#include "global.h"
#include "debug/profiler.h"
#include "render/sngl/gl_render_system.h"
#include "core/common/time.h"
#include <memory>

using namespace Sono;

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

  m_EventSystem = std::make_unique<EventSystem>();
  m_EventSystem->Init();

  m_InputSystem = std::make_unique<InputSystem>(m_MemSys->GetGlobalAllocator());
  m_InputSystem->Init();

#ifdef SN_DEBUG_PROFILER
  m_Profiler = std::make_unique<Profiler>();
  // m_Profiler->AddSinks<Sono::ConsoleProfileSink>();
  m_Profiler->AddSinks<Sono::JsonTraceSink>("profile.json");
  m_Profiler->BeginSession();
  m_Profiler->Init();
#endif

  Time::Start();
}
//--------------------------------------------------------------------------------
// shutting down in the reverse order
void Global::Shutdown() {
#ifdef SN_DEBUG_PROFILER
  S_LOG(m_Profiler->GenerateSessionReport());
  m_Profiler->EndSession();
  m_Profiler->Shutdown();
#endif

  m_InputSystem->Shutdown();

  m_EventSystem->Shutdown();

  m_RenderSystem->Shutdown();

  m_MemSys->Shutdown();
}
