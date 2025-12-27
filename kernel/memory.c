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

