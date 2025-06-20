#ifndef SN_VEC4_H
#define SN_VEC4_H

#include "vec_base.h"
#include "vec.h"

using Vec4 = Vec<4, f32>;

template <>
struct VecBase<4, f32> {
  union {
    f32 x, r, s;
  };
  union {
    f32 y, g, t;
  };
  union {
    f32 z, b, p;
  };
  union {
    f32 w, a, q;
  };

  VecBase() {}

  constexpr VecBase(f32 x, f32 y, f32 z, f32 w)
    : x(x)
    , y(y)
    , z(z)
    , w(w) {}

  f32 *ValuePtr() { return &x; }
  const f32 *ValuePtr() const { return &x; }

  static const Vec4 &Zero;
};

#endif // !SN_VEC4_H
