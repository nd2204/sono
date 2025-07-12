#ifndef SN_MOUSE_H
#define SN_MOUSE_H

#include "input_system.h"
#include "core/math/vec2.h"

enum class MouseButton { LMB, RMB, MMB, FMB, BMB };

class Mouse {
public:
  Mouse() = delete;

  static void Init(InputSystem *input = nullptr) {
    m_Input = input ? InputSystem::GetPtr() : input;
    SN_ASSERT(m_Input, "Input system is not initialized");
    input->ActivateMouse();
  }

  static Vec2 GetPosition() {
    return Vec2(m_Input->m_MouseState->posX, m_Input->m_MouseState->posY);
  }

  static Vec2 GetDelta() {
    return Vec2(m_Input->m_MouseState->deltaX, m_Input->m_MouseState->deltaY);
  }

  /// return true if mouse button is pressed (true) this frame
  static bool IsButtonDown(int button) { return m_Input->m_MouseState->currBtns.test(button); }

  /// return true if mouse button is pressed (true) this frame but not in the last frame
  static bool IsButtonPressed(int button) {
    return m_Input->m_MouseState->currBtns.test(button)
      && !m_Input->m_MouseState->prevBtns.test(button);
  }

  /// return true if mouse button is released (false) this frame but not in the last frame
  static bool IsButtonReleased(int button) {
    return !m_Input->m_MouseState->currBtns.test(button)
      && m_Input->m_MouseState->prevBtns.test(button);
  }

  static float GetScrollDelta() { return m_Input->m_MouseState->scrollDelta; }

  static bool IsScrollingUp() { return m_Input->m_MouseState->scrollDelta > 0.0f; }

  static bool IsScrollingDown() { return m_Input->m_MouseState->scrollDelta < 0.0f; }

private:
  static InputSystem *m_Input;
};

inline InputSystem *Mouse::m_Input = nullptr;

#endif // !SN_MOUSE_H
