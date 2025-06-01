#ifndef LOGGER_H
#define LOGGER_H

#include "sono/types.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>

#if defined(WIN32_BUILD)
#include <windows.h>
#endif

#define LOG_INFO_F(fmt, ...)  Logger::LogF(LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define LOG_WARN_F(fmt, ...)  Logger::LogF(LOG_LEVEL_WARNING, fmt, __VA_ARGS__)
#define LOG_ERROR_F(fmt, ...) Logger::LogF(LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define LOG_DEBUG_F(fmt, ...) Logger::LogF(LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)

#define LOG_INFO(msg)  Logger::Log(LOG_LEVEL_INFO, msg)
#define LOG_WARN(msg)  Logger::Log(LOG_LEVEL_WARNING, msg)
#define LOG_ERROR(msg) Logger::Log(LOG_LEVEL_ERROR, msg)
#define LOG_DEBUG(msg) Logger::Log(LOG_LEVEL_DEBUG, msg)

enum LogLevel {
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_NONE
};

class Logger {
public:
  static void
  Init(const std::string &filename = "", LogLevel minLevel = LOG_LEVEL_INFO) {
    minLogLevel = minLevel;
    if (!filename.empty()) {
      logFile.open(filename, std::ios::app);
    }

#if defined(WIN32_BUILD)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
  }

  static void LogF(LogLevel level, const char *fmt, ...) {
    if (level < minLogLevel)
      return;

    constexpr size_t initialSize = 1024;
    std::vector<char> buffer(initialSize);

    va_list args;
    va_start(args, fmt);
    usize needed = 0;
    vsnprintf(buffer.data(), buffer.size(), fmt, args);
    va_end(args);

    std::ostringstream oss;
    oss << GetColor(level);
    oss << "[" << GetTimestamp() << "]";
    oss << "[" << LogLevelToString(level) << "] ";
    oss << GetColor(LOG_LEVEL_NONE);
    oss << buffer.data();
    oss << "\033[0m"; // reset color

    std::string finalMessage = oss.str();

    std::cout << finalMessage << std::endl;
    if (logFile.is_open()) {
      logFile << "[" << GetTimestamp() << "][" << LogLevelToString(level)
              << "] " << fmt << std::endl;
    }
  }

  static void Log(LogLevel level, const char *message) { LogF(level, message); }

  static void Shutdown() {
    if (logFile.is_open())
      logFile.close();
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
    case LOG_LEVEL_INFO:
      return "INFO";
    case LOG_LEVEL_WARNING:
      return "WARN";
    case LOG_LEVEL_ERROR:
      return "ERROR";
    case LOG_LEVEL_DEBUG:
      return "DEBUG";
    default:
      return "UNKNOWN";
    }
  }

  static std::string GetColor(LogLevel level) {
    switch (level) {
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
  static inline LogLevel minLogLevel = LOG_LEVEL_INFO;
};

#endif // !LOGGER_H