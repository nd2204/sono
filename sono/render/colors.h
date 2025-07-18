#ifndef SN_COLORS_H
#define SN_COLORS_H

#include "core/math/vec4.h"
#include "core/math/vec3.h"

class Color : public Vec<4, f32> {
public:
  // clang-format off

  explicit Color(u32 rgba = 255)
    : Vec<4, f32>(
    ((rgba >> 24) & 255) / 255.0f,
    ((rgba >> 16) & 255) / 255.0f,
    ((rgba >> 8) & 255) / 255.0f,
    ((rgba) & 255) / 255.0f
  ) {}

  // clang-format on

  explicit Color(f32 r, f32 g, f32 b, f32 a)
    : Vec<4, f32>(r, g, b, a) {}

  explicit Color(u8 r, u8 g, u8 b, u8 a)
    : Vec<4, f32>(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f) {}

  Color(const std::string &hex)
    : Vec<4, f32>() {
    u32 rgba = ParseHexColorStr(hex);
    r = ((rgba >> 24) & 255) / 255.0f;
    g = ((rgba >> 16) & 255) / 255.0f;
    b = ((rgba >> 8) & 255) / 255.0f;
    a = ((rgba) & 255) / 255.0f;
  }

public:
  /// @brief currently supported hex string format:
  /// - [   "RRGGBBAA" |   "RRGGBB" |   "RGB" |   "RGBA" ]
  /// - [  "#RRGGBBAA" |  "#RRGGBB" |  "#RGB" |  "#RGBA" ]
  /// - [ "0xRRGGBBAA" | "0xRRGGBB" | "0xRGB" | "0xRGBA" ]
  static u32 ParseHexColorStr(const std::string &hex) {
    i32 len = hex.size();
    SN_ASSERT(len < 11 && len > 2, "Invalid hex string length");

    i32 i = 0;
    if (hex[0] == '#') i = 1;
    if (hex[0] == '0' && hex[1] == 'x') i = 2;
    int hexLen = len - i;

    u32 t, ret = 0;
    if (hexLen <= 4) {
      // shorthand format (RGBA)
      for (; i < len; i++) {
        t = HexCharToByte(hex[i]);
        t = t * 16 + t;
        ret += t << (8 * (len - i - 1));
      }
    } else {
      SN_ASSERT_F(hexLen % 2 == 0, "Invalid hex string format %s", hex.c_str());
      // expanded format (RRGGBBAA)
      for (; i < len; i += 2) {
        t = HexCharToByte(hex[i]) * 16 + HexCharToByte(hex[i + 1]);
        ret += t << (8 * ((len - i) / 2 - 1));
      }
    }
    return ret;
  }

private:
  static u8 HexCharToByte(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
    LOG_WARN_F("Invalid hex char %c", ch);
    return 0;
  }
};

class Color3 : public Vec<3, f32> {
public:
  // clang-format off

  explicit Color3(u32 rgba = 0)
    : Vec<3, f32>(
    ((rgba >> 24) & 255) / 255.0f,
    ((rgba >> 16) & 255) / 255.0f,
    ((rgba >> 8) & 255) / 255.0f
  ) {}

  // clang-format on

  explicit Color3(u8 r, u8 g, u8 b)
    : Vec<3, f32>(r / 255.0f, g / 255.0f, b / 255.0f) {}

  Color3(const std::string &hex)
    : Vec<3, f32>() {
    u32 rgba = Color::ParseHexColorStr(hex);
    r = ((rgba >> 24) & 255) / 255.0f;
    g = ((rgba >> 16) & 255) / 255.0f;
    b = ((rgba >> 8) & 255) / 255.0f;
  }

  operator Color() const { return Color(r, g, b, 1.0f); }
};

#endif // !SN_COLORS_H
