#ifndef SN_PROFILER_H
#define SN_PROFILER_H

#include "core/common/types.h"
#include "core/common/singleton.h"
#include "core/memory/allocators/arena.h"

#include <mutex>
#include <unordered_map>

// TODO: add this definition to build system as option
#define SN_DEBUG_PROFILER

#ifdef SN_DEBUG_PROFILER
#include "core/common/defines.h"
#define PROFILE_SCOPE(name) ::Sono::ProfileScope ANON_VAR(__prof)(name)
#define PROFILE_FUNCTION()  PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

namespace Sono {

/// Record time in milliseconds
struct ProfileEvent {
  const char *name;
  f32 startTime;
  f32 endTime;
  u32 threadID;
};

class IProfileSink {
public:
  virtual ~IProfileSink() = default;
  virtual void WriteHeader() = 0;
  virtual void WriteEvent(const ProfileEvent &frame) = 0;
  virtual void WriteFooter() = 0;
  virtual void Flush() = 0;
};

class ConsoleProfileSink : public IProfileSink {
public:
  void WriteHeader() override;
  void WriteEvent(const ProfileEvent &event) override;
  void WriteFooter() override;
  void Flush() override;
};

class JsonTraceSink : public IProfileSink {
public:
  JsonTraceSink(const char *filename);

  void WriteHeader() override;
  void WriteEvent(const ProfileEvent &e) override;
  void WriteFooter() override;
  void Flush() override;

private:
  std::ofstream m_File;
  bool m_First = true;
};

class Profiler : public Singleton<Profiler> {
public:
  Profiler();
  ~Profiler();

  void Init();
  void Shutdown();
  void BeginSession();
  std::string GenerateSessionReport() const;

  template <typename T, typename... Args>
  void AddSinks(Args &&...args) {
    T *sink = m_SessionAlloc.New<T>(std::forward<Args>(args)...);
    m_Sinks.push_back(sink);
  }

  void EndSession();
  b8 HasSession();
  void Record(const ProfileEvent &frame);

private:
  std::mutex m_Mutex;
  std::vector<IProfileSink *> m_Sinks;

  std::unordered_map<const char *, f32> m_EventDurationSecMap;
  ArenaAllocator m_SessionAlloc;
};

// clang-format off

/// The scope timer use for automatic scope timing
class ProfileScope {
public:
  ProfileScope(const char *name);
  ~ProfileScope();
private:
  const char *m_Name;
  f32 m_Start;
  u32 m_ThreadId;
};

} // namespace Sono

#endif // !SN_PROFILER_H
