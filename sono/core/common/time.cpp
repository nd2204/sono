#include "time.h"

using HiResClock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<f32>;

void Time::Start() {
  m_StartTime = HiResClock::now();
  m_LastTime = m_StartTime;
}

void Time::Tick() {
  auto now = HiResClock::now();

  m_DeltaTime = Duration(now - m_LastTime).count();
  m_TotalTime += m_DeltaTime;
  m_LastTime = now;

  /// TODO: move Frame count to render context or render system
  m_FrameCount++;
}

f32 Time::DeltaTime() { return m_DeltaTime; }

f32 Time::Now() { return Duration(HiResClock::now() - m_StartTime).count(); }

f32 Time::TotalTime() { return m_TotalTime; }

u64 Time::FrameCount() { return m_FrameCount; }

f32 Time::GetFPS() { return 1.0f / m_DeltaTime; }

f32 Time::m_DeltaTime = 0.01f;

f32 Time::m_TotalTime = 0.0f;

/// TODO: move Frame count to render context or render system
u64 Time::m_FrameCount = 0;

f32 Time::m_FPS = 0;

std::chrono::high_resolution_clock::time_point Time::m_LastTime;

std::chrono::high_resolution_clock::time_point Time::m_StartTime;
