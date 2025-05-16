#include "types.h"

// Example: Basic memory helper (unnecessary if libc exists)
void *memset(void *dest, int value, size_t len) {
    unsigned char *ptr = (unsigned char *)dest;
    while (len-- > 0)
        *ptr++ = (unsigned char)value;
    return dest;
}
