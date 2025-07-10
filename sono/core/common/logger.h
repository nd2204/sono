#ifndef LOGGER_H
#define LOGGER_H

#include "../common/types.h"
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>

#ifdef SONO_PLATFORM_WINDOWS
#include <windows.h>
#endif

enum LogLevel {
  LOG_LEVEL_NONE = 0,
  LOG_LEVEL_TRACE,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
};

#define LOG_F(fmt, ...)       Logger::LogF(LOG_LEVEL_NONE, fmt, __VA_ARGS__)
#define LOG_INFO_F(fmt, ...)  Logger::LogF(LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define LOG_WARN_F(fmt, ...)  Logger::LogF(LOG_LEVEL_WARNING, fmt, __VA_ARGS__)
#define LOG_ERROR_F(fmt, ...) Logger::LogF(LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define LOG_DEBUG_F(fmt, ...) Logger::LogF(LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)
#define LOG_TRACE_F(fmt, ...) Logger::LogF(LOG_LEVEL_TRACE, fmt, __VA_ARGS__)

#define S_LOG_F(fmt, ...)       Logger::LogF(LOG_LEVEL_NONE, (fmt).c_str(), __VA_ARGS__)
#define S_LOG_INFO_F(fmt, ...)  Logger::LogF(LOG_LEVEL_INFO, (fmt).c_str(), __VA_ARGS__)
#define S_LOG_WARN_F(fmt, ...)  Logger::LogF(LOG_LEVEL_WARNING, (fmt).c_str(), __VA_ARGS__)
#define S_LOG_ERROR_F(fmt, ...) Logger::LogF(LOG_LEVEL_ERROR, (fmt).c_str(), __VA_ARGS__)
#define S_LOG_DEBUG_F(fmt, ...) Logger::LogF(LOG_LEVEL_DEBUG, (fmt).c_str(), __VA_ARGS__)
#define S_LOG_TRACE_F(fmt, ...) Logger::LogF(LOG_LEVEL_TRACE, (fmt).c_str(), __VA_ARGS__)

#define LOG(msg)       Logger::Log(LOG_LEVEL_NONE, msg)
#define LOG_INFO(msg)  Logger::Log(LOG_LEVEL_INFO, msg)
#define LOG_WARN(msg)  Logger::Log(LOG_LEVEL_WARNING, msg)
#define LOG_ERROR(msg) Logger::Log(LOG_LEVEL_ERROR, msg)
#define LOG_DEBUG(msg) Logger::Log(LOG_LEVEL_DEBUG, msg)
#define LOG_TRACE(msg) Logger::LogF(LOG_LEVEL_TRACE, msg)

#define S_LOG(msg)       Logger::Log(LOG_LEVEL_NONE, (msg).c_str())
#define S_LOG_INFO(msg)  Logger::Log(LOG_LEVEL_INFO, (msg).c_str())
#define S_LOG_WARN(msg)  Logger::Log(LOG_LEVEL_WARNING, (msg).c_str())
#define S_LOG_ERROR(msg) Logger::Log(LOG_LEVEL_ERROR, (msg).c_str())
#define S_LOG_DEBUG(msg) Logger::Log(LOG_LEVEL_DEBUG, (msg).c_str())
#define S_LOG_TRACE(msg) Logger::LogF(LOG_LEVEL_TRACE, (msg).c_str())

class Logger {
public:
  static void Init(const std::string &filename = "") {
    if (!filename.empty()) {
      logFile.open(filename, std::ios::app);
    }

#ifdef SONO_PLATFORM_WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
  }

  static void LogF(LogLevel level, const char *fmt, ...) {
    if (level < LOG_LEVEL_NONE) return;

    constexpr size_t initialSize = 1024;
    std::vector<char> buffer(initialSize);

    va_list args;
    va_start(args, fmt);
    usize needed = vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);

    if (static_cast<size_t>(needed) >= buffer.size()) {
      buffer.resize(needed + 1); // +1 for null terminator

      va_start(args, fmt);
      vsnprintf(buffer.data(), buffer.size(), fmt, args);
      va_end(args);
    }

    std::ostringstream oss;
    oss << GetColor(level);
    if (level != LOG_LEVEL_NONE) {
      oss << "[" << GetTimestamp() << "]";
      oss << " [" << LogLevelToString(level) << "] ";
    }
    oss << buffer.data();

    std::string finalMessage = oss.str();

    std::cout << finalMessage << std::endl;
    if (logFile.is_open()) {
      logFile
        << "["
        << GetTimestamp()
        << "]["
        << LogLevelToString(level)
        << "] "
        << fmt
        << std::endl;
    }
  }

  static void Log(LogLevel level, const char *message) { LogF(level, message); }

  static void Shutdown() {
    if (logFile.is_open()) logFile.close();
  }

private:
  static std::string GetTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20]; // "YYYY-MM-DD HH:MM:SS"
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return buf;
  }

  static const char *LogLevelToString(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_TRACE:
      return "T";
    case LOG_LEVEL_INFO:
      return "I";
    case LOG_LEVEL_WARNING:
      return "W";
    case LOG_LEVEL_ERROR:
      return "E";
    case LOG_LEVEL_DEBUG:
      return "D";
    default:
      return "U";
    }
  }

  static std::string GetColor(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_TRACE:
      return "\033[0m"; // Default
    case LOG_LEVEL_INFO:
      return "\033[34m"; // Blue
    case LOG_LEVEL_WARNING:
      return "\033[33m"; // Yellow
    case LOG_LEVEL_ERROR:
      return "\033[31m"; // Red
    case LOG_LEVEL_DEBUG:
      return "\033[32m"; // Green
    default:
      return "\033[0m"; // Default
    }
  }

  static inline std::ofstream logFile;
};

#endif // !LOGGER_H
