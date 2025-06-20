#ifndef SN_MATH_H
#define SN_MATH_H

#define PI 3.14159265359

#include "core/common/types.h"

typedef f32 Radian;

inline Radian Radians(f32 angle) { return angle * PI / 180; }

inline f32 Degrees(Radian rad) { return rad * 180 / PI; }

#endif // !SN_MATH_H
