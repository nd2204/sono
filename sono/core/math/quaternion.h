#ifndef SN_QUATERION_H
#define SN_QUATERION_H

#include <core/math/mat4.h>
#include <core/math/vec3.h>
#include <core/common/types.h>
#include <sstream>

struct Quaternion {
  f32 x, y, z, w;

  // clang-format off
  Quaternion()
    : x(0) , y(0) , z(0) , w(1) {}

  constexpr Quaternion(const Vec3 &v, f32 s)
    : x(v.x) , y(v.y) , z(v.z) , w(s) {}

  constexpr Quaternion(f32 x, f32 y, f32 z, f32 w)
    : x(x) , y(y) , z(z) , w(w) {}
  // clang-format on
  //

  inline const f32 *ValuePtr() const { return &x; }

  inline f32 *ValuePtr() { return &x; }

  inline const Vec3 &GetVectorPart() {
    // return the reference to the first element
    return reinterpret_cast<const Vec3 &>(x);
  }

  inline Quaternion Inversed() const { return Quaternion(-x, -y, -z, w); }

  friend inline Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs) {
    // clang-format off
    return Quaternion(
      lhs.w*rhs.x + lhs.x*rhs.w + lhs.y*rhs.z - lhs.z*rhs.y,
      lhs.w*rhs.y - lhs.x*rhs.z + lhs.y*rhs.w + lhs.z*rhs.x,
      lhs.w*rhs.z + lhs.x*rhs.y - lhs.y*rhs.x + lhs.z*rhs.w,
      lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z
    );
    // clang-format on
  }

  // Vector rotation
  friend Vec3 operator*(const Quaternion &q, const Vec3 &v) {
    Quaternion qv(v.x, v.y, v.z, 0);
    Quaternion res = q * qv * q.Inversed();
    return Vec3(res.x, res.y, res.z);
  }

  inline Mat4 ToMat4() const {
    f32 xx = x * x, yy = y * y, zz = z * z;
    f32 xy = x * y, xz = x * z, yz = y * z;
    f32 wx = w * x, wy = w * y, wz = w * z;

    // clang-format off
    return Mat4(
      1 - 2 * (yy - zz),      2 * (xy + wz),     2 * (xz - wy), 0.0f,
          2 * (xy - wz), 1 -  2 * (xx - zz),     2 * (yz + wx), 0.0f,
          2 * (xz + wy),      2 * (yz - wx), 1 - 2 * (xx - yy), 0.0f,
                   0.0f,               0.0f,              0.0f, 1.0f
    );
    // clang-format on
  }

  static inline Quaternion FromAxisAngle(const Vec3 &axis, f32 angleRad) {
    Vec3 n = axis.Normalized();
    float s = sin(angleRad * 0.5f);
    return Quaternion(n.x * s, n.y * s, n.z * s, cos(angleRad * 0.5f));
  }

  static inline Quaternion FromMat3(const Mat3 &m) {
    f32 a[4] = {
      m(1, 1) - m(2, 2) - m(3, 3), // fourXSquaredMinus1
      m(2, 2) - m(1, 1) - m(3, 3), // fourYSquaredMinus1
      m(3, 3) - m(1, 1) - m(2, 2), // fourZSquaredMinus1
      m(1, 1) + m(2, 2) + m(3, 3)  // fourWSquaredMinus1
    };

    i32 biggestIdx = 0;
    for (i32 i = 1; i < 4; ++i) {
      if (a[biggestIdx] < a[i]) biggestIdx = i;
    }

    Quaternion res;
    f32 biggestVal = sqrt(a[biggestIdx] + 1.0f) * 0.5f;
    f32 mult = 0.25f / biggestVal;

    // apply the biggest component first
    res.ValuePtr()[biggestIdx] = biggestVal;

    // apply the remaining components
    switch (biggestIdx) {
      case 0:
        res.y = (m(1, 2) + m(2, 1)) * mult;
        res.z = (m(3, 1) + m(1, 3)) * mult;
        res.w = (m(2, 3) - m(3, 2)) * mult;
        break;
      case 1:
        res.x = (m(1, 2) + m(2, 1)) * mult;
        res.z = (m(2, 3) + m(3, 2)) * mult;
        res.w = (m(3, 1) - m(1, 3)) * mult;
        break;
      case 2:
        res.x = (m(3, 1) + m(1, 3)) * mult;
        res.y = (m(2, 3) + m(3, 2)) * mult;
        res.w = (m(1, 2) - m(2, 1)) * mult;
        break;
      case 3:
        res.x = (m(2, 3) - m(3, 2)) * mult;
        res.y = (m(3, 1) - m(1, 3)) * mult;
        res.z = (m(1, 2) - m(2, 1)) * mult;
        break;
    }

    return res;
  }

  static inline Quaternion FromEuler(const Vec3 &euler) {
    // clang-format off
    // Calculate half angles
    f32 cp = cos(euler.pitch * 0.5f), sp = sin(euler.pitch*0.5f);
    f32 cy = cos(euler.yaw * 0.5f),   sy = sin(euler.yaw*0.5f);
    f32 cr = cos(euler.roll * 0.5f),  sr = sin(euler.roll*0.5f);

    return Quaternion(
      cy*sp*cr + sy*cp*sr,
      sy*cp*cr - cy*sp*sr,
      cy*cp*sr - sy*sp*cr,
      cy*cp*cr + sy*sp*sr
    );
    // clang-format on
  }

  inline Vec3 ToEuler() const {
    f32 sp = -2.0f * sin(y * z - w * x);

    Vec3 v;
    // Check for gimbal lock
    if (fabs(sp) > 0.9999f) {
      // Looking straight up or down
      v.pitch = 1.570796f * sp; // PI/2 * sp
      v.yaw = atan2(-x * z + w * y, 0.5f - y * y - z * z);
      v.roll = 1.570796f * sp;
    } else {
      v.pitch = asin(sp);
      v.yaw = atan2(x * z + w * y, 0.5f - x * x - y * y);
      v.roll = atan2(x * z + w * y, 0.5f - x * x - z * z);
    }
    return v;
  }

  inline void Normalized() {
    f32 len = sqrt(x * x + y * y + z * z + w * w);
    f32 lenInv = 1 / len;
    if (len > 0.0f) {
      x *= lenInv;
      y *= lenInv;
      z *= lenInv;
      w *= lenInv;
    }
  }

  inline std::string ToString() const {
    std::ostringstream oss;
    // clang-format off
    oss << "quat["
        << std::to_string(x) << ','
        << std::to_string(y) << ','
        << std::to_string(z) << ','
        << std::to_string(w) << ']';
    // clang-format on
    return oss.str();
  }

  static inline Quaternion Identity() { return Quaternion(0, 0, 0, 1); }
};

#endif // !SN_QUATERION_H
