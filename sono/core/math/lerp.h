#ifndef SN_MATH_LERP_H
#define SN_MATH_LERP_H

#include "core/common/types.h"
#include "core/common/time.h"
#include "core/math/math.h"

template <typename T>
class Interpolated {
public:
  explicit Interpolated(
    const T &initial = {}, Sono::TransitionFn fn = Sono::Linear, f32 durationInSec = 1.0f
  )
    : m_TransitionFn(fn)
    , m_StartTime(Time::Now())
    , m_Speed(1.0f / durationInSec)
    , m_Start(initial)
    , m_End(m_Start) {}

  f32 GetElapsedSecs() const { return Time::Now() - m_StartTime; }

  T GetValue() const {
    // clang-format off
    f32 t = GetElapsedSecs() * m_Speed;
    if (t >= 1.0f) {
      return m_End;
    }
    return Sono::Lerp<T>(m_Start, m_End, m_TransitionFn(t));
    // clang-format on
  }

  void SetDuration(f32 durationInSec) { m_Speed = 1.0f / durationInSec; }

  void SetEnd(T const &endVal) {
    m_Start = GetValue();
    m_End = endVal;
    m_StartTime = Time::Now();
  }

  operator T() const { return GetValue(); };

  T &operator=(const T &endVal) {
    SetEnd(endVal);
    return m_End;
  }

private:
  Sono::TransitionFn m_TransitionFn;
  f32 m_StartTime;
  f32 m_Speed;
  T m_Start;
  T m_End;
};

#endif // !SN_MATH_LERP_H
