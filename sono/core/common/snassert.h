#ifndef ERRORS_H
#define ERRORS_H

#include "logger.h"
#include <cassert>

#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() std::raise(SIGTRAP)
#endif

#define ASSERT(expr)                                                                               \
  do {                                                                                             \
    if (!(expr)) {                                                                                 \
      LOG_F(                                                                                       \
        "Assertion Failed (at %s:%d):\n"                                                           \
        "Expression: (" #expr ") -> is null or false",                                             \
        __FILE__, __LINE__                                                                         \
      );                                                                                           \
      DEBUG_BREAK();                                                                               \
      assert(expr);                                                                                \
    }                                                                                              \
  } while (0)

/*!
 * WARNING: ONLY USE THIS MACRO WITH STRING LITERALS FOR fmt
 */
#define SN_ASSERT_F(expr, fmt, ...)                                                                \
  do {                                                                                             \
    if (!(expr)) {                                                                                 \
      LOG_F("Assertion Failed (at %s:%d): " fmt, __FILE__, __LINE__, __VA_ARGS__);                 \
      DEBUG_BREAK();                                                                               \
      assert(expr);                                                                                \
    }                                                                                              \
  } while (0)

#define SN_ASSERT(expr, msg)                                                                       \
  do {                                                                                             \
    if (!(expr)) {                                                                                 \
      LOG_F("Assertion Failed (at %s:%d): " #msg, __FILE__, __LINE__);                             \
      DEBUG_BREAK();                                                                               \
      assert(expr);                                                                                \
    }                                                                                              \
  } while (0)

#endif
