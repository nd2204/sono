#ifndef SN_MATH_H
#define SN_MATH_H

#include "core/common/types.h"

typedef f32 Radian;

namespace Sono {

constexpr f32 PI = 3.14159265358979323846;

inline Radian Radians(f32 angle) { return angle * PI / 180; }

inline f32 Degrees(Radian rad) { return rad * 180 / PI; }

} // namespace Sono

#endif // !SN_MATH_H
