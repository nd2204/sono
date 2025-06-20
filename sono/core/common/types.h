#ifndef TYPES_H
#define TYPES_H

#include <cstddef>

typedef unsigned char b8;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef size_t usize;
typedef float f32;
typedef double f64;

inline u32 PackU32(u8 a, u8 b, u8 c, u8 d) {
  return ((u32)a << 24) + ((u32)b << 16) + ((u32)c << 8) + d;
}

inline u32 PackU32(u16 ab, u16 cd) { return ((u32)ab << 16) + cd; }

#define I8_MIN  -128
#define I16_MIN ((i16)(1 << 15))
#define I32_MIN ((i32)1 << 31)
#define I64_MIN ((i64)1 << 63)

#define I8_MAX  127
#define I16_MAX ~((i16)(1 << 15))
#define I32_MAX ~((i32)1 << 31)
#define I64_MAX ~((i64)1 << 63)

#define U8_MAX    (u8)0xffU
#define U16_MAX   (u16)0xffffU
#define U32_MAX   (u32)0xffff'ffffUL
#define U64_MAX   (u64)0xffff'ffff'ffff'ffffULL
#define USIZE_MAX (usize)0xffff'ffff'ffff'ffffULL

#if defined(SONO_BUILD_DLL) && defined(SONO_PLATFORM_WINDOWS)
#define SONO_API __declspec(dllexport)
#elif defined(SONO_PLATFORM_WINDOWS)
#define SONO_API __declspec(dllimport)
#else
#define SONO_API
#endif

#endif // !TYPES_H
