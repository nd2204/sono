#ifndef SN_SYS_EVENT_DISPATCHER_H
#define SN_SYS_EVENT_DISPATCHER_H

#include "events.h"
#include <functional>
#include <vector>

class EventDispatcher {
  template <typename T>
  using EventCallbackFn = std::function<void(const T &)>;

public:
  template <typename T>
  static void Register(EventCallbackFn<T> cb) {
    m_Callbacks.emplace_back([=](const Event &e) {
      if (auto *p = std::get_if<T>(&e.payload)) {
        cb(*p);
      }
    });
  }

  static void Dispatch(const Event &e) {
    for (auto &cb : m_Callbacks) {
      cb(e);
    }
  }

private:
  static std::vector<EventCallbackFn<Event>> m_Callbacks;
};

inline std::vector<EventDispatcher::EventCallbackFn<Event>> EventDispatcher::m_Callbacks;

#endif // !SN_SYS_EVENT_DISPATCHER_H
