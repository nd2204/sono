#ifndef SN_EVENT_SYSTEM_H
#define SN_EVENT_SYSTEM_H

#include "core/common/singleton.h"
#include "events.h"

#define SN_QUEUE_EVENT(e) EventSystem::GetPtr()->Push(e);

class EventSystem : public Singleton<EventSystem> {
public:
  EventSystem();

  ~EventSystem();

  void Init();

  void Shutdown();

  void Push(const Event &e);

  Event *Pop();

private:
  static constexpr usize MAX_EVENTS = 255;

  Event m_EventQueue[MAX_EVENTS + 1];
  usize m_Head;
  usize m_Tail;
};

#endif // !SN_EVENT_SYSTEM_H
