#ifndef SN_MAT4_H
#define SN_MAT4_H

#include "core/common/types.h"
#include "mat_base.h"
#include "vec3.h"
#include "vec4.h"
#include "math.h"

struct Mat4 : public MatBase<4, 4, f32> {
  Mat4() = default;

  template <typename U>
  explicit Mat4(const U *ptr)
    : MatBase(ptr) {}

  Mat4(f32 v)
    : MatBase(v) {}

  /// @brief Construct a lookAt matrix
  /// @param pos the position of the look object
  /// @param targetPos the position of the target object
  /// @param up the vector represent the upward direction
  static Mat4 LookAt(const Vec3 &pos, const Vec3 &targetPos, const Vec3 &up);

  /// @brief Construct a perspective projection matrix
  /// @param fov The field of view in float
  /// @param aspec The aspect ratio in float
  /// @param near The distance from the origin to the near plane in float
  /// @param far The distance from the origin to the far plane in float
  static Mat4 Perspective(Radian fov, f32 aspect, f32 zNear, f32 zFar);

  /// @brief Construct a orthogonal projection matrix
  /// @param fov The field of view in float
  /// @param aspec The aspect ratio in float
  /// @param near The distance from the origin to the near plane in float
  /// @param far The distance from the origin to the far plane in float
  static Mat4 Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);

  /// @brief create a rotation matrix
  /// @params angle angle in radians
  /// @params axis the axis vector to rotate
  static Mat4 Rotation(Radian angle, const Vec3 &axis);

  /// @brief create a rotation matrix
  /// @params angle angle in radians
  /// @params axis the axis vector to rotate
  static Mat4 FromEuler(const Vec3 &euler);

  // static Mat4 Rotate(const Quaternion &rotation);

  /// @brief create a translation matrix
  /// @brief tv a translation vector
  static Mat4 Translation(const Vec3 &tv);
  static Mat4 Translation(f32 tx, f32 ty, f32 tz);

  /// @brief create a translation matrix
  /// @brief tv a translation vector
  static Mat4 Scale(const Vec3 &sv);

  // clang-format off
  Mat4(
    f32 n00, f32 n01, f32 n02, f32 n03,
    f32 n10, f32 n11, f32 n12, f32 n13,
    f32 n20, f32 n21, f32 n22, f32 n23,
    f32 n30, f32 n31, f32 n32, f32 n33
  ) {
    n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; n[0][3] = n03;
    n[1][0] = n10; n[1][1] = n11; n[1][2] = n12; n[1][3] = n13;
    n[2][0] = n20; n[2][1] = n21; n[2][2] = n22; n[2][3] = n23;
    n[3][0] = n30; n[3][1] = n31; n[3][2] = n32; n[3][3] = n33;
  }

  Mat4(const Vec4 &a, const Vec4 &b, const Vec4 &c, const Vec4 &d) {
    n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = a.w;
    n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = b.w;
    n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = c.w;
    n[3][0] = d.x; n[3][1] = d.y; n[3][2] = d.z; n[3][3] = d.w;
  }
  // clang-format on

  Mat4 &operator+=(const Mat4 &rhs) {
    n[0][0] += rhs[0][0];
    n[0][1] += rhs[0][1];
    n[0][2] += rhs[0][2];
    n[0][3] += rhs[0][3];

    n[1][0] += rhs[1][0];
    n[1][1] += rhs[1][1];
    n[1][2] += rhs[1][2];
    n[1][3] += rhs[1][3];

    n[2][0] += rhs[2][0];
    n[2][1] += rhs[2][1];
    n[2][2] += rhs[2][2];
    n[2][3] += rhs[2][3];

    n[3][0] += rhs[3][0];
    n[3][1] += rhs[3][1];
    n[3][2] += rhs[3][2];
    n[3][3] += rhs[3][3];

    return *this;
  }

  Mat4 operator+(const Mat4 &rhs) const {
    Mat4 r = *this;
    r += rhs;
    return r;
  }

  Mat4 &operator-=(const Mat4 &rhs) {
    n[0][0] -= rhs[0][0];
    n[0][1] -= rhs[0][1];
    n[0][2] -= rhs[0][2];
    n[0][3] -= rhs[0][3];

    n[1][0] -= rhs[1][0];
    n[1][1] -= rhs[1][1];
    n[1][2] -= rhs[1][2];
    n[1][3] -= rhs[1][3];

    n[2][0] -= rhs[2][0];
    n[2][1] -= rhs[2][1];
    n[2][2] -= rhs[2][2];
    n[2][3] -= rhs[2][3];

    n[3][0] -= rhs[3][0];
    n[3][1] -= rhs[3][1];
    n[3][2] -= rhs[3][2];
    n[3][3] -= rhs[3][3];

    return *this;
  }

  Mat4 operator-(const Mat4 &rhs) const {
    Mat4 r = *this;
    r -= rhs;
    return r;
  }

  // clang-format off
  
  Mat4 &operator*=(const Mat4 &rhs) {
    Mat4 lhs = *this;

    n[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0];
    n[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1];
    n[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][2];
    n[0][3] = lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3] * rhs[3][3];

    n[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0];
    n[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1];
    n[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][2];
    n[1][3] = lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3] * rhs[3][3];

    n[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0];
    n[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1];
    n[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][2];
    n[2][3] = lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3] * rhs[3][3];

    n[3][0] = lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0] + lhs[3][3] * rhs[3][0];
    n[3][1] = lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1] + lhs[3][3] * rhs[3][1];
    n[3][2] = lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2] + lhs[3][3] * rhs[3][2];
    n[3][3] = lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3] + lhs[3][3] * rhs[3][3];

    return *this;
  }

  // clang-format on

  friend inline Vec4 operator*(const Mat4 &lhs, const Vec4 &rhs) {
    return Vec4(
      lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z + lhs[0][3] * rhs.w,
      lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z + lhs[1][3] * rhs.w,
      lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z + lhs[2][3] * rhs.w,
      lhs[3][0] * rhs.x + lhs[3][1] * rhs.y + lhs[3][2] * rhs.z + lhs[3][3] * rhs.w
    );
  }

  friend inline Vec4 operator*(const Vec4 &lhs, const Mat4 &rhs) {
    return Vec4(
      rhs[0][0] * lhs.x + rhs[0][1] * lhs.y + rhs[0][2] * lhs.z + rhs[0][3] * lhs.w,
      rhs[1][0] * lhs.x + rhs[1][1] * lhs.y + rhs[1][2] * lhs.z + rhs[1][3] * lhs.w,
      rhs[2][0] * lhs.x + rhs[2][1] * lhs.y + rhs[2][2] * lhs.z + rhs[2][3] * lhs.w,
      rhs[3][0] * lhs.x + rhs[3][1] * lhs.y + rhs[3][2] * lhs.z + rhs[3][3] * lhs.w
    );
  }

  Mat4 operator*(const Mat4 &rhs) const {
    Mat4 r = *this;
    r *= rhs;
    return r;
  }

  static const Mat4 Identity;
  static const Mat4 Zero;
};

#endif // !SN_MAT4_H
