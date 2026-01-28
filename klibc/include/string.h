#ifndef CLASSICOS_KLIBC_STRING_H
#define CLASSICOS_KLIBC_STRING_H

#include <stddef.h>

extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memmove(void *dst, const void *src, size_t n);
extern void *memcpy(void *dst, const void *src, size_t n);
extern void *memset(void *dst, int c, size_t n);

extern size_t strlen(const char *s);
extern int strcmp(const char *s1, const char *s2);

#endif  // CLASSICOS_KLIBC_STRING_H
