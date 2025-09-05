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
      assert(expr);                                                                                \
      DEBUG_BREAK();                                                                               \
    }                                                                                              \
  } while (0)

#if __cplusplus >= 201103L
#define STATIC_ASSERT(expr) static_assert(expr);
#else
#define STATIC_ASSERT(expr) ASSERT(expr)
#endif

/*!
 * WARNING: ONLY USE THIS MACRO WITH STRING LITERALS FOR fmt
 */
#define SN_ASSERT_F(expr, fmt, ...)                                                                \
  do {                                                                                             \
    if (!(expr)) {                                                                                 \
      LOG_F(fmt, __VA_ARGS__);                                                                     \
      assert(expr);                                                                                \
      DEBUG_BREAK();                                                                               \
    }                                                                                              \
  } while (0)

#define SN_ASSERT(expr, msg)                                                                       \
  do {                                                                                             \
    if (!(expr)) {                                                                                 \
      assert(expr && msg);                                                                         \
      DEBUG_BREAK();                                                                               \
    }                                                                                              \
  } while (0)

#endif
