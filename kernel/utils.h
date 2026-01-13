#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Convert integer to string (base is typically 10, 16, etc.)
char* itoa(int value, char* str, int base);

// Convert unsigned integer to string (base is typically 10, 16, etc.)
char* utoa(unsigned int value, char* str, int base);

#endif // UTILS_H
