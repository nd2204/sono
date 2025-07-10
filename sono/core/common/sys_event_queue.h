#ifndef SN_SYS_EVENT_QUEUE_H
#define SN_SYS_EVENT_QUEUE_H

#include "singleton.h"
#include "sys_events.h"
#include "types.h"
#include <functional>
#include <vector>

#define SN_QUEUE_EVENT(e) SystemEventQueue::GetPtr()->Push(e);

class SystemEventQueue : public Singleton<SystemEventQueue> {
  static constexpr usize MAX_EVENTS = 255;

public:
  SystemEventQueue();

  ~SystemEventQueue();

  void Push(const Event &e);

  Event *Pop();

private:
  Event m_EventQueue[MAX_EVENTS + 1];
  usize m_Head;
  usize m_Tail;
};

class SystemEventDispatcher {
  template <typename T>
  using EventCallbackFn = std::function<void(const T &)>;

public:
  template <typename T>
  void Register(EventCallbackFn<T> callback);

  void Dispatch(const Event &event);

private:
  std::vector<EventCallbackFn<Event>> m_Callbacks;
};

#endif // !SN_SYS_EVENT_QUEUE_H
