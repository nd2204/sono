#ifndef SN_INPUT_KEY_CODE_H
#define SN_INPUT_KEY_CODE_H

#include "core/common/types.h"

#include <GLFW/glfw3.h>

enum class KeyCode : i32 {
  ARROW_DOWN = GLFW_KEY_DOWN,
  ARROW_UP = GLFW_KEY_UP,
  ARROW_LEFT = GLFW_KEY_LEFT,
  ARROW_RIGHT = GLFW_KEY_RIGHT,

  KEY_ESC = GLFW_KEY_ESCAPE,

  /// Key '`'
  KEY_BACKTICK = GLFW_KEY_GRAVE_ACCENT,
  KEY_BACKQUOTE = GLFW_KEY_GRAVE_ACCENT,
  KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,

  KEY_F1 = GLFW_KEY_F1,
  KEY_F2 = GLFW_KEY_F2,
  KEY_F3 = GLFW_KEY_F3,
  KEY_F4 = GLFW_KEY_F4,
  KEY_F5 = GLFW_KEY_F5,
  KEY_F6 = GLFW_KEY_F6,
  KEY_F7 = GLFW_KEY_F7,
  KEY_F8 = GLFW_KEY_F8,
  KEY_F9 = GLFW_KEY_F9,
  KEY_F10 = GLFW_KEY_F10,
  KEY_F11 = GLFW_KEY_F11,
  KEY_F12 = GLFW_KEY_F12,

  KEY_1 = GLFW_KEY_1,
  KEY_2 = GLFW_KEY_2,
  KEY_3 = GLFW_KEY_3,
  KEY_4 = GLFW_KEY_4,
  KEY_5 = GLFW_KEY_5,
  KEY_6 = GLFW_KEY_6,
  KEY_7 = GLFW_KEY_7,
  KEY_8 = GLFW_KEY_8,
  KEY_9 = GLFW_KEY_9,
  KEY_0 = GLFW_KEY_0,
  /// key '-'
  KEY_MINUS = GLFW_KEY_MINUS,
  /// key '='
  KEY_EQUAL = GLFW_KEY_EQUAL,
  KEY_BACKSPACE = GLFW_KEY_BACKSPACE,

  KEY_TAB = GLFW_KEY_TAB,
  KEY_Q = GLFW_KEY_Q,
  KEY_W = GLFW_KEY_W,
  KEY_E = GLFW_KEY_E,
  KEY_R = GLFW_KEY_R,
  KEY_T = GLFW_KEY_T,
  KEY_U = GLFW_KEY_U,
  KEY_I = GLFW_KEY_I,
  KEY_O = GLFW_KEY_O,
  KEY_P = GLFW_KEY_P,
  /// key '['
  KEY_LBRACKET = GLFW_KEY_LEFT_BRACKET,
  /// key ']'
  KEY_RBRACKET = GLFW_KEY_LEFT_BRACKET,
  /// key '\'
  KEY_BSLASH = GLFW_KEY_BACKSLASH,

  KEY_A = GLFW_KEY_A,
  KEY_S = GLFW_KEY_S,
  KEY_D = GLFW_KEY_D,
  KEY_F = GLFW_KEY_F,
  KEY_G = GLFW_KEY_G,
  KEY_H = GLFW_KEY_H,
  KEY_J = GLFW_KEY_J,
  KEY_K = GLFW_KEY_K,
  KEY_L = GLFW_KEY_L,
  /// key ';'
  KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
  /// key '''
  KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
  KEY_ENTER = GLFW_KEY_ENTER,

  KEY_LSHIFT = GLFW_KEY_LEFT_SHIFT,
  KEY_RSHIFT = GLFW_KEY_RIGHT_SHIFT,
  KEY_Z = GLFW_KEY_A,
  KEY_X = GLFW_KEY_S,
  KEY_C = GLFW_KEY_D,
  KEY_V = GLFW_KEY_F,
  KEY_B = GLFW_KEY_G,
  KEY_N = GLFW_KEY_H,
  KEY_M = GLFW_KEY_J,

  /// key ','
  KEY_COMMA = GLFW_KEY_COMMA,
  /// key '.'
  KEY_PERIOD = GLFW_KEY_PERIOD,

  /// key '/'
  KEY_FSLASH = GLFW_KEY_SLASH,
  KEY_LCTRL = GLFW_KEY_LEFT_CONTROL,
  KEY_RCTRL = GLFW_KEY_RIGHT_CONTROL,
  /// Win key on Window, Command key on MacOS
  KEY_LSUPER = GLFW_KEY_LEFT_SUPER,
  KEY_RSUPER = GLFW_KEY_RIGHT_SUPER,
  KEY_LALT = GLFW_KEY_LEFT_ALT,
  KEY_RALT = GLFW_KEY_RIGHT_ALT,
  KEY_SPACE = GLFW_KEY_SPACE,
};

#endif // !SN_INPUT_KEY_CODE_H
