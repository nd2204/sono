#ifndef GLOBAL_H
#define GLOBAL_H

#include "core/memory/memory_system.h"

#include <memory>

class SonoGlobal {
public:
  SonoGlobal();
  ~SonoGlobal();

private:
  std::unique_ptr<MemorySystem> m_MemSys;
};

#endif // !GLOBAL_H