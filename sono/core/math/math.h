#ifndef SN_MATH_H
#define SN_MATH_H

#include "core/common/types.h"
#include "core/common/logger.h"
#include <cmath>

typedef f32 Radian;

namespace Sono {

using TransitionFn = f32 (*)(f32);

constexpr f32 PI = 3.14159265358979323846;
// --------------------------------------------------------------------------------
constexpr inline Radian Radians(f32 angle) { return angle * PI / 180.0f; }
// --------------------------------------------------------------------------------
constexpr inline f32 Degrees(Radian rad) { return rad * 180.0f / PI; }
// --------------------------------------------------------------------------------
constexpr inline f32 Linear(f32 t) { return t; }
// --------------------------------------------------------------------------------
constexpr inline f32 EaseInSine(f32 t) { return 1.0f - std::cos((t * PI) / 2.0f); }
// --------------------------------------------------------------------------------
constexpr inline f32 EaseOutSine(f32 t) { return std::sin((t * PI) / 2.0f); }
// --------------------------------------------------------------------------------
/// -(cos(PI*t)-1) / 2
constexpr inline f32 EaseInOutSine(f32 t) { return -(std::cos(PI * t) - 1.0f) / 2.0f; }
// --------------------------------------------------------------------------------
/// t * t
constexpr inline f32 EaseInQuad(f32 t) { return t * t; }
// --------------------------------------------------------------------------------
/// 1 - (1 - t) * (1 - t)
constexpr inline f32 EaseOutQuad(f32 t) { return t * (2.0f - t); }
// --------------------------------------------------------------------------------
constexpr inline f32 EaseInOutQuad(f32 t) {
  return t < 0.5f ? 2.0f * t * t : -2.0f * t * (t - 2.0f) - 1.0f;
}
// --------------------------------------------------------------------------------
constexpr inline f32 EaseInCubic(f32 t) { return t * t * t; }
// --------------------------------------------------------------------------------
/// 1 - (1 - t) * (1 - t) * (1 - t)
constexpr inline f32 EaseOutCubic(f32 t) { return 3.0f * t - 3.0f * t * t + t * t * t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutCubic(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInQuart(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseOutQuart(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutQuart(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInQuint(f32 t) { return t; }
// --------------------------------------------------------------------------------
/// 1 - (1 - t)^5
inline f32 EaseOutQuint(f32 t) { return 1.0f - std::powf(1.0f - t, 5.0f); }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutQuint(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInExpo(f32 t) { return t; }
// --------------------------------------------------------------------------------
inline f32 EaseOutExpo(f32 t) { return t == 1.0f ? 1.0f : 1.0f - std::powf(2.0f, -10.0f * t); }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutExpo(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInCirc(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseOutCirc(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutCirc(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInBack(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseOutBack(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutBack(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInElastic(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseOutElastic(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutElastic(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInBounce(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseOutBounce(f32 t) { return t; }
// --------------------------------------------------------------------------------
// TODO:
constexpr inline f32 EaseInOutBounce(f32 t) { return t; }
// --------------------------------------------------------------------------------
template <typename T>
constexpr inline T Lerp(T a, T b, f32 t) {
  return a + (b - a) * t;
}

} // namespace Sono

#endif // !SN_MATH_H
