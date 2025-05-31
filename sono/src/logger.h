#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

#if defined(WIN32_BUILD)
#include <windows.h>
#endif

#define LOG_INFO(msg)  Logger::Log(LOG_LEVEL_INFO, msg)
#define LOG_WARN(msg)  Logger::Log(LOG_LEVEL_WARNING, msg)
#define LOG_ERROR(msg) Logger::Log(LOG_LEVEL_ERROR, msg)
#define LOG_DEBUG(msg) Logger::Log(LOG_LEVEL_DEBUG, msg)

enum LogLevel {
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARNING,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_DEBUG
};

class Logger {
public:
  static void
  Init(const std::string &filename = "", LogLevel minLevel = LOG_LEVEL_INFO) {
    minLogLevel = minLevel;
    if (!filename.empty()) {
      logFile.open(filename, std::ios::app);
    }

#if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
  }

  static void Log(LogLevel level, const std::string &message) {
    if (level < minLogLevel)
      return;

    std::ostringstream oss;
    oss << GetColor(level);
    oss << "[" << GetTimestamp() << "] ";
    oss << "[" << LogLevelToString(level) << "] ";
    oss << message;
    oss << "\033[0m"; // reset color

    std::string finalMessage = oss.str();

    std::cout << finalMessage << std::endl;
    if (logFile.is_open()) {
      logFile << "[" << GetTimestamp() << "] [" << LogLevelToString(level)
              << "] " << message << std::endl;
    }
  }

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
      return "INFO ";
    case LOG_LEVEL_WARNING:
      return "WARN ";
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
      return "\033[32m"; // Green
    case LOG_LEVEL_WARNING:
      return "\033[33m"; // Yellow
    case LOG_LEVEL_ERROR:
      return "\033[31m"; // Red
    case LOG_LEVEL_DEBUG:
      return "\033[36m"; // Cyan
    default:
      return "\033[0m"; // Default
    }
  }

  static inline std::ofstream logFile;
  static inline LogLevel minLogLevel = LOG_LEVEL_INFO;
};

#endif // !LOGGER_H