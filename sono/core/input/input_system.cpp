#include "input_system.h"
#include "mouse.h"
#include "core/debug/profiler.h"
#include <variant>

// --------------------------------------------------------------------------------
InputSystem::InputSystem(Allocator &allocator)
  : m_Allocator(allocator)
  , m_MouseState(nullptr)
  , m_KeyState(nullptr) {}
// --------------------------------------------------------------------------------
void InputSystem::Init() {
  LOG_INFO("<-- Initializing InputSystem -->");
  Mouse::Init(this);
}
// --------------------------------------------------------------------------------
void InputSystem::Shutdown() {
  LOG_INFO("<-- Shutting down InputSystem -->");
  m_Allocator.Free(m_MouseState);
  m_Allocator.Free(m_KeyState);
}
// --------------------------------------------------------------------------------
void InputSystem::InjectEvent(const Event &e) {
  switch (e.type) {
    case EventType::KEY: {
      ASSERT(std::holds_alternative<KeyEvent>(e.payload));
      auto &k = std::get<KeyEvent>(e.payload);
      ActivateKeyboard();
      if (k.key >= 0 && k.key < 512) {
        m_KeyState->currKeys.set(k.key, k.down);
      }
      break;
    }
    case EventType::MOUSE_MOVE: {
      auto &k = std::get<MouseMoveEvent>(e.payload);
      ActivateMouse();

      if (m_FirstMouse) {
        m_MouseState->prePosX = k.xpos;
        m_MouseState->prePosY = k.ypos;
        m_MouseState->deltaX = 0;
        m_MouseState->deltaY = 0;
        m_FirstMouse = false;
      } else {
        m_MouseState->posX = k.xpos;
        m_MouseState->posY = k.ypos;

        // Accumulate delta value since each
        // frame might generate more than one mouse position
        m_MouseState->deltaX += (m_MouseState->posX - m_MouseState->prePosX);
        m_MouseState->deltaY += (m_MouseState->posY - m_MouseState->prePosY);

        m_MouseState->prePosX = m_MouseState->posX;
        m_MouseState->prePosY = m_MouseState->posY;
      }

      break;
    }
    case EventType::MOUSE_BUTTON: {
      ASSERT(std::holds_alternative<MouseButtonEvent>(e.payload));
      auto &k = std::get<MouseButtonEvent>(e.payload);
      ActivateMouse();
      m_MouseState->currBtns.set(k.button, k.pressed);
      break;
    }
    case EventType::MOUSE_SCROLL: {
      ASSERT(std::holds_alternative<MouseScrollEvent>(e.payload));
      auto &k = std::get<MouseScrollEvent>(e.payload);
      ActivateMouse();
      m_MouseState->scrollDelta = k.scrollY - m_MouseState->scrollY;
      m_MouseState->scrollY = k.scrollY;
      break;
    }
    case EventType::TEXT:
    case EventType::QUIT:
    case EventType::WINDOW_RESIZE:
      break;
  }
}
// --------------------------------------------------------------------------------
void InputSystem::ActivateMouse() {
  if (!m_MouseState) {
    m_MouseState = m_Allocator.New<MouseState>();
  }
}
// --------------------------------------------------------------------------------
void InputSystem::ActivateKeyboard() {
  if (!m_KeyState) {
    m_KeyState = m_Allocator.New<KeyState>();
  }
}
// --------------------------------------------------------------------------------
void InputSystem::EndFrame() {
  if (m_MouseState) {
    m_MouseState->deltaX = 0;
    m_MouseState->deltaY = 0;
    m_MouseState->scrollDelta = 0;
    m_MouseState->prevBtns = m_MouseState->currBtns;
  }

  if (m_KeyState) {
    m_KeyState->prevKeys = m_KeyState->currKeys;
  }
}
