#include "time.h"

void Time::Tick() {
  using clock = std::chrono::high_resolution_clock;
  auto now = clock::now();

  if (m_FrameCount == 0) {
    m_LastTime = now;
  } else {
    m_DeltaTime = std::chrono::duration<f32>(now - m_LastTime).count();
    m_TotalTime += m_DeltaTime;
    m_LastTime = now;
  }

  m_FrameCount++;
  m_FPS = 1.0f / m_DeltaTime;
}

f32 Time::DeltaTime() { return m_DeltaTime; }
f32 Time::TotalTime() { return m_TotalTime; }
u64 Time::FrameCount() { return m_FrameCount; }
f32 Time::GetFPS() { return m_FPS; }

f32 Time::m_DeltaTime = 0.01f;
f32 Time::m_TotalTime = 0.0f;
u64 Time::m_FrameCount = 0;
f32 Time::m_FPS = 0;

std::chrono::high_resolution_clock::time_point Time::m_LastTime;