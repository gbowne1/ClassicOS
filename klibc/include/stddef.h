#ifndef CLASSICOS_KLIBC_STDDEF_H
#define CLASSICOS_KLIBC_STDDEF_H

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#undef NULL
#define NULL ((void*)0)

#endif  // CLASSICOS_KLIBC_STDDEF_H
