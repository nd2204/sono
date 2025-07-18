#ifndef SN_VEC_H
#define SN_VEC_H

#include "vec_base.h"
#include <ostream>
#include <sstream>

template <int N, typename T>
struct Vec : public VecBase<N, T> {
  using VecBase<N, T>::ValuePtr;

  // =========================================================================================
  // Ctors
  // =========================================================================================
  Vec() {}

  constexpr Vec(T x, T y, T z, T w)
    : VecBase<N, T>(x, y, z, w) {}

  constexpr Vec(T x, T y, T z)
    : VecBase<N, T>(x, y, z) {}

  constexpr Vec(T x, T y)
    : VecBase<N, T>(x, y) {}

  explicit Vec(T v) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] = v;
    }
  }
  // =========================================================================================
  // Array subscripts
  // =========================================================================================

  T &operator[](i32 i) { return ValuePtr()[i]; }
  // -----------------------------------------------------------------------------------------
  const T &operator[](i32 i) const { return ValuePtr()[i]; }

  // =========================================================================================
  // Unary Exprs
  // =========================================================================================

  Vec operator-() const { return (*this) * -1; }
  // -----------------------------------------------------------------------------------------
  const Vec &operator+() const { return *this; }

  // =========================================================================================
  // Vector arithmetics
  // =========================================================================================

  Vec &operator+=(f32 rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] += rhs;
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator-=(f32 rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] -= rhs;
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator*=(f32 rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] *= rhs;
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator/=(f32 rhs) {
    rhs = 1.0f / rhs; // Inverse
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] *= rhs;
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  friend constexpr Vec operator+(const Vec &lhs, f32 rhs) {
    Vec result;
    for (int i = 0; i < N; i++) {
      result[i] = lhs[i] + rhs;
    }
    return result;
  }
  // -----------------------------------------------------------------------------------------
  friend constexpr Vec operator-(const Vec &lhs, f32 rhs) {
    Vec result;
    for (int i = 0; i < N; i++) {
      result[i] = lhs[i] - rhs;
    }
    return result;
  }
  // -----------------------------------------------------------------------------------------
  friend constexpr Vec operator*(const Vec &lhs, f32 rhs) {
    Vec result;
    for (int i = 0; i < N; i++) {
      result[i] = lhs[i] * rhs;
    }
    return result;
  }
  // -----------------------------------------------------------------------------------------
  friend constexpr Vec operator/(const Vec &lhs, f32 rhs) {
    Vec result;
    rhs = 1.0f / rhs;
    for (int i = 0; i < N; i++) {
      result[i] = lhs[i] * rhs;
    }
    return result;
  }

  // =========================================================================================
  // Vector, vector operations
  // =========================================================================================

  Vec &operator+=(const Vec<N, T> &rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] += rhs[i];
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator-=(const Vec<N, T> &rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] -= rhs[i];
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator*=(const Vec<N, T> &rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] *= rhs[i];
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec &operator/=(const Vec<N, T> &rhs) {
    for (int i = 0; i < N; i++) {
      ValuePtr()[i] /= rhs[i];
    }
    return *this;
  }
  // -----------------------------------------------------------------------------------------
  Vec operator+(const Vec<N, T> &rhs) const {
    Vec result = *this;
    result += rhs;
    return result;
  }
  // -----------------------------------------------------------------------------------------
  Vec operator-(const Vec<N, T> &rhs) const {
    Vec result = *this;
    result -= rhs;
    return result;
  }
  // -----------------------------------------------------------------------------------------
  Vec operator*(const Vec<N, T> &rhs) const {
    Vec result = *this;
    result *= rhs;
    return result;
  }
  // -----------------------------------------------------------------------------------------
  Vec operator/(const Vec<N, T> &rhs) const {
    Vec result = *this;
    result /= rhs;
    return result;
  }
  // -----------------------------------------------------------------------------------------
  bool operator==(const Vec<N, T> &other) const {
    T *data = ValuePtr();
    for (int i = 0; i < N; i++) {
      if (other[i] != data[i]) return false;
    }
    return true;
  }
  // -----------------------------------------------------------------------------------------
  constexpr T Dot(const Vec<N, T> &rhs) const {
    T result = T{};
    for (int i = 0; i < N; ++i) {
      result += ValuePtr()[i] * rhs[i];
    }
    return result;
  }

  // =========================================================================================
  // Utilities Functions
  // =========================================================================================

  T Normalize() {
    T len = Length();
    if (len > T(0.0f)) {
      *this /= len;
    }
    return len;
  }
  // -----------------------------------------------------------------------------------------
  f32 Dist(const Vec &rhs) const { return (*this - rhs).Length(); }
  // -----------------------------------------------------------------------------------------
  f32 DistSquared(const Vec &rhs) const { return (*this - rhs).LengthSquared(); }
  // -----------------------------------------------------------------------------------------
  constexpr T LengthSquared() const { return Dot(*this); }
  // -----------------------------------------------------------------------------------------
  T Length() const { return std::sqrt(LengthSquared()); }
  // -----------------------------------------------------------------------------------------
  Vec<N, T> Normalized() const {
    Vec<N, T> result = *this;
    result.Normalize();
    return result;
  }

  std::string ToString() const {
    std::stringstream ss;
    ss << "vec" << N << "[";
    for (int i = 0; i < N - 1; i++) {
      ss << std::to_string(ValuePtr()[i]) << ",";
    }
    ss << std::to_string(ValuePtr()[N - 1]) << "]";
    return ss.str();
  }

  size_t hash() const {
    size_t hash = 0x811c9dc5;
    const f32 *p = ValuePtr();
    for (int i = 0; i < N; i++) {
      hash = (hash ^ (size_t)p[i]) * 0x01000193;
    }
    return hash;
  }
};

namespace std {

template <int N, typename T>
struct hash<Vec<N, T>> {
  size_t operator()(const Vec<N, T> &v) const { return v.hash(); }
};

} // namespace std

#endif // !SN_VEC_H
