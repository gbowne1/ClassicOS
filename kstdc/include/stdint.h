#ifndef __CLASSIC_OS_KSTDC_STDINT_H__
#define __CLASSIC_OS_KSTDC_STDINT_H__ 1

typedef unsigned char uint8_t;
typedef short unsigned int uint16_t;
typedef unsigned int uint32_t;
typedef long unsigned int uint64_t;

typedef unsigned char uint_least8_t;
typedef short unsigned int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef long unsigned int uint_least64_t;

typedef unsigned char uint_fast8_t;
typedef long unsigned int uint_fast16_t;
typedef long unsigned int uint_fast32_t;
typedef long unsigned int uint_fast64_t;

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;

typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
typedef long int int_least64_t;

typedef signed char int_fast8_t;
typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;

typedef long unsigned int uintptr_t;
typedef long int intptr_t;

typedef long unsigned int uintmax_t;
typedef long int intmax_t;

#define INT8_C(x) ((int8_t)(x))
#define INT16_C(x) ((int16_t)(x))
#define INT32_C(x) ((int32_t)(x))
#define INT64_C(x) ((int64_t)(x))

#define UINT8_C(x) ((uint8_t)(x))
#define UINT16_C(x) ((uint16_t)(x))
#define UINT32_C(x) ((uint32_t)(x))
#define UINT64_C(x) ((uint64_t)(x))

#define INTMAX_C(x) ((intmax_t)(x))
#define UINTMAX_C(x) ((uintmax_t)(x))

#define UINT8_MAX 0xff
#define UINT16_MAX 0xffff
#define UINT32_MAX 0xffffffffU
#define UINT64_MAX 0xffffffffffffffffUL

#define INT8_MAX 0x7f
#define INT16_MAX 0x7fff
#define INT32_MAX 0x7fffffff
#define INT64_MAX 0x7fffffffffffffffL

#define INT8_MIN (-INT8_MAX - 1)
#define INT16_MIN (-INT16_MAX - 1)
#define INT32_MIN (-INT32_MAX - 1)
#define INT64_MIN (-INT64_MAX - 1)

#define UINT_LEAST8_MAX 0xff
#define UINT_LEAST16_MAX 0xffff
#define UINT_LEAST32_MAX 0xffffffffU
#define UINT_LEAST64_MAX 0xffffffffffffffffUL

#define INT_LEAST8_MAX 0x7f
#define INT_LEAST16_MAX 0x7fff
#define INT_LEAST32_MAX 0x7fffffff
#define INT_LEAST64_MAX 0x7fffffffffffffffL

#define INT_LEAST8_MIN (-INT_LEAST8_MAX - 1)
#define INT_LEAST16_MIN (-INT_LEAST16_MAX - 1)
#define INT_LEAST32_MIN (-INT_LEAST32_MAX - 1)
#define INT_LEAST64_MIN (-INT_LEAST64_MAX - 1)

#define UINT_FAST8_MAX 0xff
#define UINT_FAST16_MAX 0xffffffffffffffffUL
#define UINT_FAST32_MAX 0xffffffffffffffffUL
#define UINT_FAST64_MAX 0xffffffffffffffffUL

#define INT_FAST8_MAX 0x7f
#define INT_FAST16_MAX 0x7fffffffffffffffL
#define INT_FAST32_MAX 0x7fffffffffffffffL
#define INT_FAST64_MAX 0x7fffffffffffffffL

#define INT_FAST8_MIN (-INT_FAST8_MAX - 1)
#define INT_FAST16_MIN (-INT_FAST16_MAX - 1)
#define INT_FAST32_MIN (-INT_FAST32_MAX - 1)
#define INT_FAST64_MIN (-INT_FAST64_MAX - 1)

#define UINTPTR_MAX 0xffffffffffffffffUL
#define INTPTR_MAX 0x7fffffffffffffffL
#define INTPTR_MIN (-INTPTR_MAX - 1)

#define UINTMAX_MAX 0xffffffffffffffffUL
#define INTMAX_MAX 0x7fffffffffffffffL
#define INTMAX_MIN (-INTMAX_MAX - 1)

#define PTRDIFF_MAX 0x7fffffffffffffffL
#define PTRDIFF_MIN (-PTRDIFF_MAX - 1)

#define SIG_ATOMIC_MAX 0x7fffffff
#define SIG_ATOMIC_MIN (-SIG_ATOMIC_MAX - 1)

#define SIZE_MAX 0xffffffffffffffffUL

#define WCHAR_MAX 0x7fffffff
#define WCHAR_MIN (-WCHAR_MAX - 1)

#define WINT_MAX 0xffffffffU
#define WINT_MIN (-WINT_MAX - 1)

#endif // __CLASSIC_OS_KSTDC_STDINT_H__
