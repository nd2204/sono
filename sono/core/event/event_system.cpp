#include "event_system.h"

// --------------------------------------------------------------------------------
EventSystem::EventSystem()
  : m_Head(0)
  , m_Tail(0) {}
// --------------------------------------------------------------------------------
EventSystem::~EventSystem() {}
// --------------------------------------------------------------------------------
void EventSystem::Init() { LOG_INFO("<-- Initializing Event System -->"); }
// --------------------------------------------------------------------------------
void EventSystem::Shutdown() { LOG_INFO("<-- Shutting down Event System -->"); }
// --------------------------------------------------------------------------------
void EventSystem::Push(const Event &e) {
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
Event *EventSystem::Pop() {
  if (m_Tail >= m_Head) {
    return nullptr;
  }
  return &m_EventQueue[m_Tail++ & MAX_EVENTS];
}
