#ifndef SN_VEC3_H
#define SN_VEC3_H

#include <core/math/vec_base.h>
#include <core/math/math.h>
#include <core/math/vec.h>

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

namespace Sono {
// --------------------------------------------------------------------------------
constexpr inline Vec3 Radians(const Vec3 &euler) {
  f32 mult = PI / 180.0f;
  return Vec3(euler.x * mult, euler.y * mult, euler.z * mult);
}
// --------------------------------------------------------------------------------
constexpr inline Vec3 Degrees(const Vec3 &euler) {
  f32 mult = 180.0f / PI;
  return Vec3(euler.x * mult, euler.y * mult, euler.z * mult);
}

} // namespace Sono

#endif // !SN_VEC3_H
