#include "core/common/time.h"
#include "profiler.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <ios>
#include <mutex>
#include <sstream>
#include <thread>

#ifndef SONO_PLATFORM_WINDOW
#include <unistd.h>
#endif // SONO_PLATFORM_WINDOW

using namespace Sono;

// ================================================================================
// Profiler
// ================================================================================

Profiler::Profiler()
  : m_SessionAlloc((sizeof(ConsoleProfileSink) + sizeof(JsonTraceSink)) * 4) {}

Profiler::~Profiler() {}

void Profiler::Init() {}

void Profiler::Shutdown() { m_SessionAlloc.FreeInternalBuffer(); };

void Profiler::BeginSession() {
  for (auto *sink : m_Sinks) {
    sink->WriteHeader();
  }
}
// --------------------------------------------------------------------------------
void Profiler::Record(const ProfileEvent &event) {
  for (auto *sink : m_Sinks) {
    sink->WriteEvent(event);
  }

  if (m_EventDurations.find(event.name) != m_EventDurations.end()) {
    m_EventDurations[event.name] += event.end - event.start;
  } else {
    m_EventDurations[event.name] = event.end - event.start;
  }
}
// --------------------------------------------------------------------------------
f32 Profiler::GetEventDuration(const char *name) const {
  if (m_EventDurations.find(name) != m_EventDurations.end()) {
    return m_EventDurations.at(name);
  }
  return 0.0f;
}
// --------------------------------------------------------------------------------
std::string Profiler::GenerateSessionReport() const {
  // Calculate total time
  f32 totalTime = 0.0f;
  i32 maxStrLen = 0;
  for (const auto &[name, time] : m_EventDurations) {
    totalTime += time;
    maxStrLen = std::max<i32>(strlen(name), maxStrLen);
  }

  // Sort entries by time descending
  std::vector<std::pair<const char *, f32>> sortedEvents(
    m_EventDurations.begin(), m_EventDurations.end()
  );
  std::sort(sortedEvents.begin(), sortedEvents.end(), [](const auto &a, const auto &b) {
    return a.second > b.second;
  });

  // Build the report string
  std::ostringstream oss;
  oss << "Top CPU Hotspots:\n";

  for (const auto &[name, time] : sortedEvents) {
    f32 percentage = (totalTime > 0.0f) ? (time / totalTime * 100.0f) : 0.0f;
    oss
      << "|__ "
      << std::left
      << std::setw(maxStrLen + 1)
      << name
      << " - "
      << std::fixed
      << std::setprecision(2)
      << Sono::FormatSeconds(time)
      << " ("
      << percentage
      << "%)\n";
  }

  return oss.str();
}
// --------------------------------------------------------------------------------
void Profiler::EndSession() {
  for (auto *sink : m_Sinks) {
    sink->WriteFooter();
    sink->Flush();
  }
  m_Sinks.clear();
  m_SessionAlloc.Clear();
}
// --------------------------------------------------------------------------------
b8 Profiler::HasSession() { return false; }

// ================================================================================
// ProfileScope
// ================================================================================

ProfileScope::ProfileScope(const char *name)
  : m_Name(name)
  , m_Start(Time::Now())
  , m_ThreadId(std::hash<std::thread::id>{}(std::this_thread::get_id())) {}
// --------------------------------------------------------------------------------
ProfileScope::~ProfileScope() {
  auto end = Time::Now();
  ProfileEvent e = {m_Name, m_Start, end, m_ThreadId};
  Profiler::Get().Record(std::move(e));
}

// ================================================================================
// ConsoleProfileSink
// ================================================================================

void ConsoleProfileSink::WriteHeader() {}
// --------------------------------------------------------------------------------
void ConsoleProfileSink::WriteEvent(const ProfileEvent &e) {
  LOG_TRACE_F("%s: %.2fms", e.name, e.Duration(TimeUnit::MILISECONDS));
}
// --------------------------------------------------------------------------------
void ConsoleProfileSink::WriteFooter() {}
// --------------------------------------------------------------------------------
void ConsoleProfileSink::Flush() { fflush(stdout); }

// ================================================================================
// JsonTraceSink
// ================================================================================

JsonTraceSink::JsonTraceSink(const char *filename) {
  m_File.open(filename);
  ASSERT(m_File.is_open());
}
// --------------------------------------------------------------------------------
void JsonTraceSink::WriteHeader() {
  m_File
    << "{\n"
    << "\"otherData\": {},\n"
    << "\"traceEvents\": [\n";
  m_File.flush();
  m_First = true;
}
// --------------------------------------------------------------------------------
void JsonTraceSink::WriteEvent(const ProfileEvent &e) {
  if (!m_First) m_File << ",\n";
  m_First = false;
  // clang-format off
  m_File
    << "{"
       "\"cat\": \"PERF\","
       "\"name\":\"" << e.name << "\","
       "\"ph\": \"B\"," 
       "\"ts\":" << Sono::FormatSecondsToUnit(e.start, TimeUnit::MILISECONDS) << ","
       "\"pid\":" << getpid() << ","
       "\"tid\":" << e.threadID
    << "},{"
       "\"cat\": \"PERF\","
       "\"name\":\"" << e.name << "\","
       "\"ph\": \"E\"," 
       "\"ts\":" << Sono::FormatSecondsToUnit(e.end, TimeUnit::MILISECONDS) << ","
       "\"pid\":" << getpid() << ","
       "\"tid\":" << e.threadID
    << "}";
  m_File.flush();
  // clang-format on
}
// --------------------------------------------------------------------------------
void JsonTraceSink::WriteFooter() {
  m_File << "\n]\n}\n";
  m_File.flush();
}
// --------------------------------------------------------------------------------
void JsonTraceSink::Flush() {
  m_File.flush();
  m_File.close();
}
