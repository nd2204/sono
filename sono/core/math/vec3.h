#ifndef SN_VEC3_H
#define SN_VEC3_H

#include "vec_base.h"
#include "vec.h"

typedef Vec<3, f32> Vec3;

template <>
struct VecBase<3, f32> {
  union {
    f32 x, r, s, pitch;
  };
  union {
    f32 y, g, t, yaw;
  };
  union {
    f32 z, b, p, roll;
  };

  VecBase() {}

  constexpr VecBase(f32 x, f32 y, f32 z)
    : x(x)
    , y(y)
    , z(z) {}

  f32 *ValuePtr() { return &x; }
  const f32 *ValuePtr() const { return &x; }

  Vec3 Cross(const Vec3 &rhs) const {
    // clang-format off
    return Vec3(
      y * rhs.z - z * rhs.y,
      z * rhs.x - x * rhs.z,
      x * rhs.y - y * rhs.x
    );
    // clang-format on
  }

  static const Vec3 &Up;
  static const Vec3 &Down;
  static const Vec3 &Left;
  static const Vec3 &Right;
  static const Vec3 &Forward;
  static const Vec3 &Back;
  static const Vec3 &Zero;
};

#endif // !SN_VEC3_H
