#ifndef SN_SYS_EVENTS_H
#define SN_SYS_EVENTS_H

#include "snassert.h"
#include "types.h"
#include <variant>

enum class EventType {
  KEY,
  MOUSE_MOVE,
  MOUSE_BUTTON,
  MOUSE_SCROLL,
  TEXT,
  QUIT,
  WINDOW_RESIZE,
};

struct KeyEvent {
  i32 key;
  b8 down;

  std::string ToString() const {
    std::ostringstream oss;
    oss << "KeyEvent{" << std::to_string(key) << "," << (down ? "true" : "false") << "}";
    return oss.str();
  }
};

struct MouseMoveEvent {
  f64 xpos, ypos;

  std::string ToString() const {
    std::ostringstream oss;
    oss << "MouseMoveEvent{" << std::to_string(xpos) << "," << std::to_string(ypos) << "}";
    return oss.str();
  }
};

struct MouseButtonEvent {
  i32 button;
  b8 pressed;

  std::string ToString() const {
    std::ostringstream oss;
    oss
      << "MouseButtonEvent{"
      << std::to_string(button)
      << ","
      << (pressed ? "true" : "false")
      << "}";
    return oss.str();
  }
};

struct MouseScrollEvent {
  f32 scrollX, scrollY;

  std::string ToString() const {
    std::ostringstream oss;
    oss << "MouseScrollEvent{" << std::to_string(scrollX) << "," << std::to_string(scrollY) << "}";
    return oss.str();
  }
};

struct WindowResizeEvent {
  i32 width, height;

  std::string ToString() const {
    std::ostringstream oss;
    oss << "WindowResizeEvent{" << std::to_string(width) << "," << std::to_string(height) << "}";
    return oss.str();
  }
};

// clang-format off

using EventPayload = std::variant<
  std::monostate,
  KeyEvent,
  MouseMoveEvent,
  MouseButtonEvent,
  MouseScrollEvent,
  WindowResizeEvent
>;

// clang-format on

struct Event {
  i64 time;
  EventType type;
  EventPayload payload;

  // NOTE: Temporary for debugging and logging
  std::string ToString() const {
    return std::visit(
      [&](const auto &p) -> std::string {
        using T = std::decay_t<decltype(p)>;
        if constexpr (std::is_same_v<T, std::monostate>)
          return "Payload: None";
        else
          return p.ToString();
      },
      payload
    );
  }
};

#endif // !SN_SYS_EVENTS_H
