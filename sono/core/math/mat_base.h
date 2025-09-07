#ifndef SN_MAT_BASE_H
#define SN_MAT_BASE_H

#include "core/common/snassert.h"
#include <cstring>
#include <sstream>
#include <string>
#include <utility>

// template <class MatrixType, u32 N, u32 C, u32 R, u32 I>
// struct TransposeMatrixMTImpl {
//   enum {
//     NextI = I + 1,
//     NextC = NextI / N % N,
//     NextR = (NextI % N) + NextC + 1,
//   };
//   static inline void eval(MatrixType &m) {
//     if (R < N) {
//       std::swap(m[C][R], m[R][C]);
//     }
//     TransposeMatrixMTImpl<MatrixType, NextI, NextC, NextR, NextI>::eval(m);
//   }
// };
//
// // Templatized matrix transpose
// template <class MatrixType, u32 N>
// struct TransposeMatrixMT {
//   static inline void eval(MatrixType &m) { TransposeMatrixMTImpl<MatrixType, N, 0, 1,
//   0>::eval(m); }
// };

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

  /// @brief setup an identity matrix with the value of v in the diagonal cells
  explicit MatBase(T v) {
    SN_ASSERT(R == C, "Must be a square matrix");
    SN_ASSERT(R > 1 || C > 1, "Matrix must have at least 2 row and column");
    memset(n, 0, R * C * sizeof(T));
    for (int i = 0; i < R; i++) {
      n[i][i] = v;
    }
  }

  T *operator[](usize row) {
    SN_ASSERT(row < R, "Index out of bound");
    return n[row];
  }

  const T *operator[](usize row) const {
    SN_ASSERT(row < R, "Index out of bound");
    return n[row];
  }

  const T &operator()(usize row, usize col) const {
    SN_ASSERT(row < R && col < C, "Index out of bound");
    return n[row][col];
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "mat" << R << "x" << C << "[";
    for (int i = 0; i < R; ++i) {
      ss << "[";
      for (int j = 0; j < C; ++j) {
        ss << std::to_string(n[i][j]);
        if (j < C - 1) ss << ",";
      }
      ss << "]";
      if (i < R - 1) ss << ",";
    }
    ss << "]";
    return ss.str();
  }
};

#endif // !SN_MAT_BASE_H
