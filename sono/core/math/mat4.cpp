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

Mat4 Mat4::Perspective(f32 fov, f32 aspect, f32 near, f32 far) {
  // aspect*1/tan(fov/2)                   0                   0                   0
  //                   0        1/tan(fov/2)                   0                   0
  //                   0                   0             f/(f-n)        (-f*n)/(f-n)
  //                   0                   0                   1                   0
  Mat4 m = Mat4::Zero;
  f32 f = fov/2;
  f32 d = far - near;
  m[0][0] = aspect * (1/tan(f));
  m[1][1] = 1 / tan(f);
  m[2][2] = far / d;
  m[2][3] = -1.0f * (far * near) / d;
  m[3][2] = 1;
  return m;
}

Mat4 Mat4::Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
  // 2/(right-left)                 0                0    -right-left/right-left
  //              0    2/(top-bottom)                0    -top-bottom/top-bottom
  //              0                 0    -2/(far-near)        -far-near/far-near
  //              0                 0                0                         1
  Mat4 m = Mat4::Identity;
  m[0][0] = 2/(right-left);
  m[0][3] = (-right-left)/(right-left);
  m[1][1] = 2/(top-bottom);
  m[1][3] = (-top-bottom)/(top-bottom);
  m[2][2] = -2/(far-near);
  m[2][3] = (-far-near)/(far-near);
  return Mat4();
}
