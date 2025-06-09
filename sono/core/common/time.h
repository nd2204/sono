#ifndef TIME_H
#define TIME_H

#include "types.h"
#include <chrono>

class Time {
public:
  static void Tick();
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
  static std::chrono::high_resolution_clock::time_point m_FpsTime;
};

#endif // !TIME_H