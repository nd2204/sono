#include "core/common/time.h"
#include "profiler.h"
#include <cstdio>
#include <ios>
#include <mutex>
#include <thread>

template <>
Sono::Profiler *Singleton<Sono::Profiler>::m_sInstance = nullptr;

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
  LOG_TRACE_F("%s: %.2fms", e.name, (e.endTime - e.startTime) * 1000);
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
       "\"ts\":" << e.startTime * 1000 << ","
       "\"pid\":" << getpid() << ","
       "\"tid\":" << e.threadID
    << "},{"
       "\"cat\": \"PERF\","
       "\"name\":\"" << e.name << "\","
       "\"ph\": \"E\"," 
       "\"ts\":" << e.endTime * 1000 << ","
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
void JsonTraceSink::Flush() { m_File.close(); }
