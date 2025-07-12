#ifndef GLOBAL_H
#define GLOBAL_H

#include "common/singleton.h"
#include "event/event_system.h"
#include "core/memory/memory_system.h"
#include "core/input/input_system.h"
#include "render/render_system.h"
#include "debug/profiler.h"

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
  std::unique_ptr<EventSystem> m_EventSystem;
  std::unique_ptr<InputSystem> m_InputSystem;

#ifdef SN_DEBUG_PROFILER
  std::unique_ptr<Profiler> m_Profiler;
#endif

  // TODO: replace this with free list allocator or other
  ArenaAllocator m_Allocator;
};

} // namespace Sono

#endif // !GLOBAL_H
