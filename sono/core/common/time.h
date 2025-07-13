#ifndef TIME_H
#define TIME_H

#include "types.h"
#include <chrono>
#include <iomanip>
#include <sstream>

class Time {
public:
  static void Tick();

  static void Start();

  static f32 Now();

  static f32 DeltaTime();

  static f32 TotalTime();

  static u64 FrameCount();

  static f32 GetFPS();

private:
  static f32 m_DeltaTime;
  static f32 m_TotalTime;
  static u64 m_FrameCount;
  static f32 m_FPS;

  static std::chrono::high_resolution_clock::time_point m_LastTime;
  static std::chrono::high_resolution_clock::time_point m_StartTime;
};

enum TimeUnit {
  NANOSECONDS = 0,
  MILISECONDS = 1,
  SECONDS = 2,
  MINUTES = 3,
  HOURS = 4,
  DAYS = 5,
};

constexpr const char *TIME_UNIT_STR[] = {
  "ns", "ms", "s", "m", "h", "d",
};

namespace Sono {

inline f32 FormatSecondsToUnit(f32 seconds, TimeUnit unit) {
  f32 formattedTime = seconds;
  i32 currentUnit = TimeUnit::SECONDS;
  while (formattedTime < 1.0f && unit < currentUnit) {
    formattedTime *= 1000.0f;
    currentUnit--;
  }
  while (formattedTime >= 60.0f && unit > currentUnit) {
    formattedTime *= 0.0166666667f; // currentTime * 1/60
    currentUnit++;
  }
  return formattedTime;
}

inline std::string FormatSeconds(f32 seconds) {
  std::ostringstream oss;
  i32 currentUnit = TimeUnit::SECONDS;
  f32 currentTime = seconds;
  while (currentTime < 1.0f) {
    currentTime *= 1000.0f;
    currentUnit--;
  }
  while (currentTime >= 60.0f) {
    currentTime *= 0.0166666667f; // currentTime * 1/60
    currentUnit++;
  }
  oss << std::to_string(currentTime) << TIME_UNIT_STR[currentUnit];
  return oss.str();
}

} // namespace Sono

#endif // !TIME_H
