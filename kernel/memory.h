#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint8_t */

#ifdef __cplusplus
extern "C" {
#endif

/* C11 / POSIX-2004 signatures */
void *memcpy(void *restrict dst, const void *restrict src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
int   memcmp(const void *s1, const void *s2, size_t n);

/* Optional fast-path using 32-bit loads (x86 only) */
#if defined(__i386__) && !defined(MEMORY_NO_OPT)
# define MEMORY_OPTIMIZED 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H */
