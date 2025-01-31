#ifndef __CLASSIC_OS_KSTDC_STDDEF_H__
#define __CLASSIC_OS_KSTDC_STDDEF_H__ 1

typedef long unsigned int size_t;
typedef long int ptrdiff_t;

#ifndef __cplusplus

typedef int wchar_t;

#endif

#ifdef __cplusplus

typedef decltype(nullptr) nullptr_t;

#endif

#ifndef __cplusplus

#define NULL ((void *)0)

#else

#define NULL 0

#endif

#undef offsetof
#define offsetof(s, m) __builtin_offsetof(s, m)

#endif // __CLASSIC_OS_KSTDC_STDDEF_H__
