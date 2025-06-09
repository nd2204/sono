#include "global.h"

//--------------------------------------------------------------------------------
SonoGlobal::SonoGlobal() {
  m_MemSys = std::make_unique<MemorySystem>();
  m_MemSys->Init();
}

//--------------------------------------------------------------------------------
SonoGlobal::~SonoGlobal() { m_MemSys->Shutdown(); }