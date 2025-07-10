#ifndef SN_VEC_BASE_H
#define SN_VEC_BASE_H

#include <cmath>
#include <core/common/snassert.h>

template <int N, typename T>
struct VecBase {
  T data[N];
  T *ValuePtr() { return data; }
  const T *ValuePtr() const { return data; }

  // Default constructor for Vec struct
  VecBase() {}

  VecBase(const VecBase<N, T> &other) {
    for (int i = 0; i < N; i++) {
      data[i] = other.data[i];
    }
  }

  VecBase &operator=(const VecBase<N, T> &other) {
    for (int i = 0; i < N; i++) {
      data[i] = other.data[i];
    }
  }

  constexpr VecBase(T x, T y, T z, T w) {
    SN_ASSERT_F(N > 2, "expected at least 4 dimensions (current: %d)", N);
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
  }

  constexpr VecBase(T x, T y, T z) {
    SN_ASSERT_F(N > 2, "expected at least 3 dimensions (current: %d)", N);
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  constexpr VecBase(T x, T y) {
    SN_ASSERT_F(N > 2, "expected at least 2 dimensions (current: %d)", N);
    data[0] = x;
    data[1] = y;
  }
};

#endif // !SN_VEC_BASE_H
