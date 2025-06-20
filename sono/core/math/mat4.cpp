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

const Mat4 Mat4::Identity {
  1.0f, 0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 1.0f,
};

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

Mat4 Mat4::Scale(const Vec3 &sv) {
  Mat4 scale(1.0f);
  scale[0][0] = sv.x;
  scale[1][1] = sv.y;
  scale[2][2] = sv.z;
  return scale;
}

Mat4 Mat4::Translation(const Vec3 &tv) {
  Mat4 trans(1.0f);
  trans[3][0] = tv.x;
  trans[3][1] = tv.y;
  trans[3][2] = tv.z;
  return trans;
}

Mat4 Mat4::Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar) {
  // aspect*1/tan(fov/2)                   0                   0                   0
  //                   0        1/tan(fov/2)                   0                   0
  //                   0                   0             f/(f-n)        (-f*n)/(f-n)
  //                   0                   0                   1                   0
  Mat4 m = Mat4::Zero;
  f32 f = fov/2;
  f32 d = zFar - zNear;
  m[0][0] = aspect * (1/tan(f));
  m[1][1] = 1 / tan(f);
  m[2][2] = zFar / d;
  m[2][3] = -1.0f * (zFar * zNear) / d;
  m[3][2] = 1;
  return m;
}

Mat4 Mat4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar) {
  // 2/(right-left)                 0                0    -right-left/right-left
  //              0    2/(top-bottom)                0    -top-bottom/top-bottom
  //              0                 0    -2/(far-near)        -far-near/far-near
  //              0                 0                0                         1
  Mat4 m = Mat4::Identity;
  m[0][0] = 2/(right-left);
  m[0][3] = (-right-left)/(right-left);
  m[1][1] = 2/(top-bottom);
  m[1][3] = (-top-bottom)/(top-bottom);
  m[2][2] = -2/(zFar-zNear);
  m[2][3] = (-zFar-zNear)/(zFar-zNear);
  return Mat4();
}
