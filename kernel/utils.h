#ifndef UTILS_H
#define UTILS_H

#include "types.h"

// Convert integer to string (base is typically 10, 16, etc.)
char* itoa(int value, char* str, int base);

// Convert unsigned integer to string (base is typically 10, 16, etc.)
char* utoa(unsigned int value, char* str, int base);

int memcmp(const void *ptr1, const void *ptr2, size_t num);
void *memset(void *dest, int value, size_t len);

#endif // UTILS_H
