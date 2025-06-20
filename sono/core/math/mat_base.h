#ifndef SN_MAT_BASE_H
#define SN_MAT_BASE_H

#include "core/common/snassert.h"
#include <cstring>
#include <sstream>
#include <string>

template <int R, int C, typename T>
struct MatBase {
  T n[R][C];

  T *ValuePtr() { return &n[0][0]; }
  const T *ValuePtr() const { return &n[0][0]; }

  MatBase() {}

  template <typename U>
  explicit MatBase(const U *ptr) {
    SN_ASSERT(R > 1 || C > 1, "Matrix must have at least 2 row and column");
    for (int i = 0; i < R; i++) {
      for (int j = 0; j < C; j++) {
        n[i][j] = T(ptr[i * 4 + j]);
      }
    }
  }

  template <typename U>
  explicit MatBase(const MatBase<R, C, U> &o)
    : MatBase(o.ValuePtr()) {}

  T *operator[](usize row) {
    ASSERT(row < R);
    return n[row];
  }

  const T *operator[](usize row) const {
    ASSERT(row < R);
    return n[row];
  }

  /// @brief setup an identity matrix with the value of v in the diagonal cells
  explicit MatBase(T v) {
    SN_ASSERT(R == C, "Must be a square matrix");
    SN_ASSERT(R > 1 || C > 1, "Matrix must have at least 2 row and column");
    memset(n, 0, R * C * sizeof(T));
    for (int i = 0; i < R; i++) {
      n[i][i] = v;
    }
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "mat" << R << "x" << C << "[";
    for (int i = 0; i < R - 1; i++) {
      ss << "[";
      for (int j = 0; j < C - 1; j++) {
        ss << n[i][j] << ",";
      }
      ss << n[i][C - 1] << "]";
    }
    ss << "[";
    for (int j = 0; j < C - 1; j++) {
      ss << n[R - 1][j] << ",";
    }
    ss << n[R - 1][C - 1] << "]]";
    return ss.str();
  }
};

#endif // !SN_MAT_BASE_H
