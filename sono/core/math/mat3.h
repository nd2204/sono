#ifndef SN_MAT3_H
#define SN_MAT3_H

#include "core/common/types.h"
#include "mat_base.h"
#include "vec3.h"

struct Mat3 : public MatBase<3, 3, f32> {
  Mat3() = default;

  template <typename U>
  explicit Mat3(const U *ptr)
    : MatBase(ptr) {}

  Mat3(f32 v)
    : MatBase(v) {}

  // clang-format off
  Mat3(
    f32 n00, f32 n01, f32 n02,
    f32 n10, f32 n11, f32 n12,
    f32 n20, f32 n21, f32 n22
  ) {
    n[0][0] = n00; n[0][1] = n01; n[0][2] = n02;
    n[1][0] = n10; n[1][1] = n11; n[1][2] = n12;
    n[2][0] = n20; n[2][1] = n21; n[2][2] = n22;
  }

  Mat3(const Vec3 &a, const Vec3 &b, const Vec3 &c) {
    n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z;
    n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z;
    n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z;
  }
  // clang-format on

  Mat3 operator+(const Mat3 &rhs) {
    Mat3 r;

    r[0][0] = n[0][0] + rhs[0][0];
    r[0][1] = n[0][1] + rhs[0][1];
    r[0][2] = n[0][2] + rhs[0][2];

    r[1][0] = n[1][0] + rhs[1][0];
    r[1][1] = n[1][1] + rhs[1][1];
    r[1][2] = n[1][2] + rhs[1][2];

    r[2][0] = n[2][0] + rhs[2][0];
    r[2][1] = n[2][1] + rhs[2][1];
    r[2][2] = n[2][2] + rhs[2][2];

    return r;
  }

  Mat3 operator-(const Mat3 &rhs) {
    Mat3 r;

    r[0][0] = n[0][0] - rhs[0][0];
    r[0][1] = n[0][1] - rhs[0][1];
    r[0][2] = n[0][2] - rhs[0][2];

    r[1][0] = n[1][0] - rhs[1][0];
    r[1][1] = n[1][1] - rhs[1][1];
    r[1][2] = n[1][2] - rhs[1][2];

    r[2][0] = n[2][0] - rhs[2][0];
    r[2][1] = n[2][1] - rhs[2][1];
    r[2][2] = n[2][2] - rhs[2][2];

    return r;
  }

  Mat3 operator*(const Mat3 &rhs) {
    Mat3 r;

    r[0][0] = n[0][0] * rhs[0][0] + n[0][1] * rhs[1][0] + n[0][2] * rhs[2][0];
    r[0][1] = n[0][0] * rhs[0][1] + n[0][1] * rhs[1][1] + n[0][2] * rhs[2][1];
    r[0][2] = n[0][0] * rhs[0][2] + n[0][1] * rhs[1][2] + n[0][2] * rhs[2][2];

    r[1][0] = n[1][0] * rhs[0][0] + n[1][1] * rhs[1][0] + n[1][2] * rhs[2][0];
    r[1][1] = n[1][0] * rhs[0][1] + n[1][1] * rhs[1][1] + n[1][2] * rhs[2][1];
    r[1][2] = n[1][0] * rhs[0][2] + n[1][1] * rhs[1][2] + n[1][2] * rhs[2][2];

    r[2][0] = n[2][0] * rhs[0][0] + n[2][1] * rhs[1][0] + n[2][2] * rhs[2][0];
    r[2][1] = n[2][0] * rhs[0][1] + n[2][1] * rhs[1][1] + n[2][2] * rhs[2][1];
    r[2][2] = n[2][0] * rhs[0][2] + n[2][1] * rhs[1][2] + n[2][2] * rhs[2][2];

    return r;
  }

  static const Mat3 Zero;
};

inline Vec3 operator*(const Mat3 &lhs, const Vec3 &rhs) {
  return Vec3(
    lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z,
    lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z,
    lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z
  );
}

inline Vec3 operator*(const Vec3 &lhs, const Mat3 &rhs) {
  return Vec3(
    rhs[0][0] * lhs.x + rhs[0][1] * lhs.y + rhs[0][2] * lhs.z,
    rhs[1][0] * lhs.x + rhs[1][1] * lhs.y + rhs[1][2] * lhs.z,
    rhs[2][0] * lhs.x + rhs[2][1] * lhs.y + rhs[2][2] * lhs.z
  );
}

#endif // !SN_MAT3_H
