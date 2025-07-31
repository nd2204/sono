#ifndef SN_DEFINES_H
#define SN_DEFINES_H

#define SN_TO_STR_LITERAL(x) #x

#define CONCAT_IMPL(x, y) x##y

#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define ANON_VAR(prefix) CONCAT(prefix, __LINE__)

#define GET_ENUM_NAME_STR(init, name, ...) #name

#define GENERATE_ENUM(init, name, ...) name = init

#define GENERATE_STRING(init, name, ...) GET_ENUM_NAME_STR(init, name, ...)

#define DEFINE_ENUMS(NAME, FOREACH_MACRO) enum NAME { FOREACH_MACRO(GENERATE_ENUM) }

#define DEFINE_STRING_CONSTANTS(NAME, FOREACH_MACRO)                                               \
  const char *NAME[] = {FOREACH_MACRO(GENERATE_STRING)}

#define BITVAL(n) (1 << (n - 1))

#define MASKTOBIT(n) ((1 << (n)) - 1)

#define BIT_ON(x, n) ((x) & BITVAL(n))

#define ENABLE_BIT(x, n) (x) |= BITVAL(n)

#define DISABLE_BIT(x, n) (x) &= ~BITVAL(n)

#define TOGGLE_BIT(x, n) (x) ^= BITVAL(n)

#define SET_BIT(x, n, v) (x) |= (BITVAL(n) * (v % 2)) & (BITVAL(n) * (v % 2))

#endif // !SN_DEFINES_H
