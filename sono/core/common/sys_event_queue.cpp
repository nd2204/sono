#include "sys_event_queue.h"
#include "logger.h"
#include <variant>

template <>
SystemEventQueue *Singleton<SystemEventQueue>::m_sInstance = nullptr;

// ================================================================================
// SystemEventQueue
// ================================================================================

SystemEventQueue::SystemEventQueue()
  : m_Head(0)
  , m_Tail(0) {}
// --------------------------------------------------------------------------------
SystemEventQueue::~SystemEventQueue() {}
// --------------------------------------------------------------------------------
void SystemEventQueue::Push(const Event &e) {
  Event *ev = &m_EventQueue[m_Head & MAX_EVENTS];
  if (m_Head - m_Tail >= MAX_EVENTS) {
    LOG_ERROR("SystemEventQueue: events overflow");
    m_Tail++;
  }
  m_Head++;

  ev->time = e.time;
  ev->type = e.type;
  ev->payload = e.payload;
}
// --------------------------------------------------------------------------------
Event *SystemEventQueue::Pop() {
  if (m_Tail >= m_Head) {
    return nullptr;
  }
  return &m_EventQueue[m_Tail++ & MAX_EVENTS];
}

// ================================================================================
// SystemEventDispatcher
// ================================================================================

template <typename T>
void SystemEventDispatcher::Register(SystemEventDispatcher::EventCallbackFn<T> cb) {
  m_Callbacks.emplace_back([=](const Event &e) {
    if (auto p = std::get_if<T>(e.payload)) {
      cb(e);
    }
  });
}
// --------------------------------------------------------------------------------
void SystemEventDispatcher::Dispatch(const Event &e) {
  for (auto &cb : m_Callbacks) {
    cb(e);
  }
}
