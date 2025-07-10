#ifndef SN_VEC2_H
#define SN_VEC2_H

#include "vec_base.h"
#include "vec.h"

typedef Vec<2, f32> Vec2;

template <>
struct VecBase<2, f32> {
  f32 x, y;

  VecBase() {}

  constexpr VecBase(f32 x, f32 y)
    : x(x)
    , y(y) {}

  f32 *ValuePtr() { return &x; }
  const f32 *ValuePtr() const { return &x; }

  constexpr f32 Cross(const Vec<2, f32> &rhs) { return x * rhs.y - y * rhs.x; }

  static const Vec2 &Up;
  static const Vec2 &Down;
  static const Vec2 &Left;
  static const Vec2 &Right;
  static const Vec2 &Zero;
};

#endif // !SN_VEC2_H
