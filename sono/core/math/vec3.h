#ifndef SN_VEC3_H
#define SN_VEC3_H

#include "vec_base.h"
#include "vec.h"

typedef Vec<3, float> Vec3;

template <>
struct VecBase<3, f32> {
  union {
    f32 x, r, s;
  };
  union {
    f32 y, g, t;
  };
  union {
    f32 z, b, p;
  };

  VecBase() {}

  constexpr VecBase(f32 x, f32 y, f32 z)
    : x(x)
    , y(y)
    , z(z) {}

  f32 *ValuePtr() { return &x; }
  const f32 *ValuePtr() const { return &x; }

  Vec3 Cross(const Vec3 &rhs) const;

  static const Vec3 &Up;
  static const Vec3 &Down;
  static const Vec3 &Left;
  static const Vec3 &Right;
  static const Vec3 &Forward;
  static const Vec3 &Back;
  static const Vec3 &Zero;
};

inline Vec3 VecBase<3, f32>::Cross(const Vec3 &rhs) const {
  return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

#endif // !SN_VEC3_H
