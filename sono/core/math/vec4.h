#include "vec_base.h"
#include "vec.h"

using Vec4 = Vec<4, f32>;

template <>
struct VecBase<4, f32> {
  f32 x, y, z, w;

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
