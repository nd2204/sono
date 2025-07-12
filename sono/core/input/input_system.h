#ifndef SN_INPUT_SYSTEM_H
#define SN_INPUT_SYSTEM_H

#include "core/common/singleton.h"
#include "core/event/events.h"
#include "core/memory/allocator.h"

#include <bitset>

struct MouseState {
  f64 posX, posY;
  f64 prePosX, prePosY;
  f64 deltaX, deltaY;

  f32 scrollY = 0.0f;
  f32 prevScrollY = 0.0f;
  f32 scrollDelta = 0.0f;

  std::bitset<8> currBtns;
  std::bitset<8> prevBtns;
};

struct KeyState {
  std::bitset<512> currKeys;
  std::bitset<512> prevKeys;
};

class InputSystem : public Singleton<InputSystem> {
public:
  InputSystem(Allocator &allocator);

  ~InputSystem() = default;

  void Init();

  void Shutdown();

  void InjectEvent(const Event &e);

  void EndFrame();

  void ActivateMouse();

  void ActivateKeyboard();

public:
  Allocator &m_Allocator;
  MouseState *m_MouseState;
  KeyState *m_KeyState;

  b8 m_FirstMouse = true;
};

#endif // !SN_INPUT_SYSTEM_H
