#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *c1 = s1;
    const unsigned char *c2 = s2;
    int d = 0;

    while (n--) {
        d = (int)*c1++ - (int)*c2++;
        if (d) break;
    }

    return d;
}

void *memmove(void *dst, const void *src, size_t n) {
    const char *p = src;
    char *q = dst;
#if defined(__i386__) || defined(__x86_64__)
    if (q < p) {
        __asm__ volatile("cld; rep; movsb" : "+c"(n), "+S"(p), "+D"(q));
    } else {
        p += (n - 1);
        q += (n - 1);
        __asm__ volatile("std; rep; movsb; cld" : "+c"(n), "+S"(p), "+D"(q));
    }
#else
    if (q < p) {
        while (n--) {
            *q++ = *p++;
        }
    } else {
        p += n;
        q += n;
        while (n--) {
            *--q = *--p;
        }
    }
#endif

    return dst;
}

void *memcpy(void *dst, const void *src, size_t n) {
    const char *p = src;
    char *q = dst;
#if defined(__i386__)
    size_t nl = n >> 2;
    __asm__ volatile("cld ; rep ; movsl ; movl %3,%0 ; rep ; movsb"
                     : "+c"(nl), "+S"(p), "+D"(q)
                     : "r"(n & 3));
#elif defined(__x86_64__)
    size_t nq = n >> 3;
    __asm__ volatile("cld ; rep ; movsq ; movl %3,%%ecx ; rep ; movsb"
                     : "+c"(nq), "+S"(p), "+D"(q)
                     : "r"((uint32_t)(n & 7)));
#else
    while (n--) {
        *q++ = *p++;
    }
#endif

    return dst;
}

void *memset(void *dst, int c, size_t n) {
    char *q = dst;

#if defined(__i386__)
    size_t nl = n >> 2;
    __asm__ volatile("cld ; rep ; stosl ; movl %3,%0 ; rep ; stosb"
                     : "+c"(nl), "+D"(q)
                     : "a"((unsigned char)c * 0x01010101U), "r"(n & 3));
#elif defined(__x86_64__)
    size_t nq = n >> 3;
    __asm__ volatile("cld ; rep ; stosq ; movl %3,%%ecx ; rep ; stosb"
                     : "+c"(nq), "+D"(q)
                     : "a"((unsigned char)c * 0x0101010101010101U),
                       "r"((uint32_t)n & 7));
#else
    while (n--) {
        *q++ = c;
    }
#endif

    return dst;
}

size_t strlen(const char *s) {
    const char *ss = s;
    while (*ss) ss++;
    return ss - s;
}

int strcmp(const char *s1, const char *s2) {
    const unsigned char *c1 = (const unsigned char *)s1;
    const unsigned char *c2 = (const unsigned char *)s2;
    unsigned char ch;
    int d = 0;

    while (1) {
        d = (int)(ch = *c1++) - (int)*c2++;
        if (d || !ch) break;
    }

    return d;
}

char *strncpy(char *dst, const char *src, size_t n) {
    char *q = dst;
    const char *p = src;
    char ch;

    while (n) {
        n--;
        *q++ = ch = *p++;
        if (!ch) break;
    }

    memset(q, 0, n);

    return dst;
}
