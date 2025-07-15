#include "mat4.h"
#include <cmath>

// TODO: Implement custom matrix manipulation

// clang-format off

const Mat4 Mat4::Zero {
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,
};
// --------------------------------------------------------------------------------
const Mat4 Mat4::Identity {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f,
};
// --------------------------------------------------------------------------------
Mat4 Mat4::Rotation(Radian angle, const Vec3 &axis) {
  Vec3 a = axis.Normalized();
  float c = std::cos(angle);
  float s = std::sin(angle);
  float oneMinusC = 1.0f - c;

  float x = a.x;
  float y = a.y;
  float z = a.z;

  Mat4 rot(1.0f);
  rot[0][0] = c + x * x * oneMinusC;
  rot[1][0] = x * y * oneMinusC - z * s;
  rot[2][0] = x * z * oneMinusC + y * s;

  rot[0][1] = y * x * oneMinusC + z * s;
  rot[1][1] = c + y * y * oneMinusC;
  rot[2][1] = y * z * oneMinusC - x * s;

  rot[0][2] = z * x * oneMinusC - y * s;
  rot[1][2] = z * y * oneMinusC + x * s;
  rot[2][2] = c + z * z * oneMinusC;
  return rot;
}
// --------------------------------------------------------------------------------
Mat4 Mat4::RotationFromEuler(const Vec3 &eulerAngle) {
  float cx = cosf(eulerAngle.pitch), sx = sinf(eulerAngle.pitch);
  float cy = cosf(eulerAngle.yaw),   sy = sinf(eulerAngle.yaw);
  float cz = cosf(eulerAngle.roll),  sz = sinf(eulerAngle.roll);

  // Column-major order
  return Mat4{
    cz * cy,  cz * sy * sx - sz * cx,   cz * sy * cx + sz * sx,   0.0f,
    sz * cy,  sz * sy * sx + cz * cx,   sz * sy * cx - cz * sx,   0.0f,
    -sy,      cy * sx,                  cy * cx,                  0.0f,
    0.0f,     0.0f,                     0.0f,                     1.0f
  };
}
// --------------------------------------------------------------------------------
Mat4 Mat4::Scale(const Vec3 &sv) {
  Mat4 scale(1.0f);
  scale[0][0] = sv.x;
  scale[1][1] = sv.y;
  scale[2][2] = sv.z;
  return scale;
}
// --------------------------------------------------------------------------------
Mat4 Mat4::Translation(f32 tx, f32 ty, f32 tz) {
  Mat4 trans(1.0f);
  trans[3][0] = tx;
  trans[3][1] = ty;
  trans[3][2] = tz;
  return trans;
}
// --------------------------------------------------------------------------------
Mat4 Mat4::Translation(const Vec3 &tv) {
  Mat4 trans(1.0f);
  trans[3][0] = tv.x;
  trans[3][1] = tv.y;
  trans[3][2] = tv.z;
  return trans;
}
// --------------------------------------------------------------------------------
Mat4 Mat4::LookAt(const Vec3 &P, const Vec3 &T, const Vec3 &U) {
  Vec3 _D = (P - T).Normalized();
  Vec3 _R = (U.Cross(_D)).Normalized();
  Vec3 _U = _D.Cross(_R);
  return Mat4(
          _R.x,       _U.x,       _D.x, 0.0f,
          _R.y,       _U.y,       _D.y, 0.0f,
          _R.z,       _U.z,       _D.z, 0.0f,
    -_R.Dot(P), -_U.Dot(P), -_D.Dot(P), 1.0f
  );
}
// --------------------------------------------------------------------------------
Mat4 Mat4::Perspective(Radian fov, f32 aspect, f32 zNear, f32 zFar) {
  // Row major order matrix
  // 1/(aspect*tan(fov/2))                   0                   0                   0
  //                     0        1/tan(fov/2)                   0                   0
  //                     0                   0        -(f+n)/(f-n)        -(2fn)/(f-n)
  //                     0                   0                  -1                   0

  // Column major order matrix
  Mat4 m(Mat4::Zero);
  f32 tanHalfFov = tan(fov * 0.5f); // fov must be in radians!
  m[0][0] = 1.0f / (aspect * tanHalfFov);
  m[1][1] = 1.0f / tanHalfFov;
  m[2][2] = -(zFar + zNear) / (zFar - zNear);
  m[2][3] = -1.0f;
  m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
  return m;
}
// --------------------------------------------------------------------------------
Mat4 Mat4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar) {
  // Row major order matrix 
  // 2/(right-left)                 0                0    -right-left/right-left
  //              0    2/(top-bottom)                0    -top-bottom/top-bottom
  //              0                 0    -2/(far-near)        -far-near/far-near
  //              0                 0                0                         1

  // Column major order matrix
  Mat4 m = Mat4::Identity;
  m[0][0] = 2/(right-left);
  m[0][3] = (-right-left)/(right-left);
  m[1][1] = 2/(top-bottom);
  m[1][3] = (-top-bottom)/(top-bottom);
  m[2][2] = -2/(zFar-zNear);
  m[2][3] = (-zFar-zNear)/(zFar-zNear);
  return Mat4();
}
