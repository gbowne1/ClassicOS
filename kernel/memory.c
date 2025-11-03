#include "memory.h"

/* note: this is a stub, please use care as theres duplicate functions in utils implementation 
/* --------------------------------------------------------------------- *
 *  Helper: copy a single byte (used by both memcpy and memmove)
 * --------------------------------------------------------------------- */
static inline void byte_copy_forward(uint8_t *dst, const uint8_t *src, size_t n)
{
    while (n--) *dst++ = *src++;
}

static inline void byte_copy_backward(uint8_t *dst, const uint8_t *src, size_t n)
{
    dst += n; src += n;
    while (n--) *--dst = *--src;
}

/* --------------------------------------------------------------------- *
 *  memcpy  –  no overlap allowed (behaviour undefined if overlap)
 * --------------------------------------------------------------------- */
void *memcpy(void *restrict dst, const void *restrict src, size_t n)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

#if defined(MEMORY_OPTIMIZED)
    /* Align destination to 4-byte boundary */
    size_t align = (uintptr_t)d & 3U;
    if (align) {
        size_t head = 4 - align;
        if (head > n) head = n;
        byte_copy_forward(d, s, head);
        d += head; s += head; n -= head;
    }

    /* 32-bit word copy – safe because we already aligned dst */
    {
        uint32_t *d32 = (uint32_t *)d;
        const uint32_t *s32 = (const uint32_t *)s;
        size_t words = n / 4;
        while (words--) *d32++ = *s32++;
        d = (uint8_t *)d32;
        s = (const uint8_t *)s32;
        n &= 3;
    }
#endif

    byte_copy_forward(d, s, n);
    return dst;
}

/* --------------------------------------------------------------------- *
 *  memmove – handles overlapping regions correctly
 * --------------------------------------------------------------------- */
void *memmove(void *dst, const void *src, size_t n)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

    if (n == 0 || dst == src)
        return dst;

    if (d < s) {                     /* copy forward */
#if defined(MEMORY_OPTIMIZED)
        /* Same fast path as memcpy when no overlap */
        size_t align = (uintptr_t)d & 3U;
        if (align) {
            size_t head = 4 - align;
            if (head > n) head = n;
            byte_copy_forward(d, s, head);
            d += head; s += head; n -= head;
        }
        {
            uint32_t *d32 = (uint32_t *)d;
            const uint32_t *s32 = (const uint32_t *)s;
            size_t words = n / 4;
            while (words--) *d32++ = *s32++;
            d = (uint8_t *)d32;
            s = (const uint8_t *)s32;
            n &= 3;
        }
#endif
        byte_copy_forward(d, s, n);
    } else {                         /* copy backward */
        byte_copy_backward(d, s, n);
    }
    return dst;
}

/* --------------------------------------------------------------------- *
 *  memcmp  – lexicographical compare
 * --------------------------------------------------------------------- */
int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *a = (const uint8_t *)s1;
    const uint8_t *b = (const uint8_t *)s2;

#if defined(MEMORY_OPTIMIZED)
    /* Align to 4-byte boundary */
    size_t align = (uintptr_t)a & 3U;
    if (align && align == ((uintptr_t)b & 3U)) {
        size_t head = 4 - align;
        if (head > n) head = n;
        while (head--) {
            int diff = *a++ - *b++;
            if (diff) return diff;
        }
        n -= head;
    }

    {
        const uint32_t *a32 = (const uint32_t *)a;
        const uint32_t *b32 = (const uint32_t *)b;
        size_t words = n / 4;
        while (words--) {
            uint32_t va = *a32++, vb = *b32++;
            if (va != vb) {
                /* byte-wise fallback for the differing word */
                const uint8_t *pa = (const uint8_t *)(a32 - 1);
                const uint8_t *pb = (const uint8_t *)(b32 - 1);
                for (int i = 0; i < 4; ++i) {
                    int diff = pa[i] - pb[i];
                    if (diff) return diff;
                }
            }
        }
        a = (const uint8_t *)a32;
        b = (const uint8_t *)b32;
        n &= 3;
    }
#endif

    while (n--) {
        int diff = *a++ - *b++;
        if (diff) return diff;
    }
    return 0;
}
