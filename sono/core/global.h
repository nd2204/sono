#ifndef GLOBAL_H
#define GLOBAL_H

#include "common/singleton.h"
#include "core/memory/memory_system.h"
#include "render/render_system.h"

#include <memory>

namespace Sono {

class Global : public Singleton<Global> {
public:
  Global();
  ~Global();

  void Init();
  void Shutdown();

private:
  std::unique_ptr<MemorySystem> m_MemSys;
  std::unique_ptr<RenderSystem> m_RenderSystem;
};

} // namespace Sono

#endif // !GLOBAL_H
