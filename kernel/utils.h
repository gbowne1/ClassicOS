#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>


// Convert integer to string (base is typically 10, 16, etc.)
char* itoa(int value, char* str, int base);

// Convert unsigned integer to string (base is typically 10, 16, etc.)
char* utoa(unsigned int value, char* str, int base);

void print_hex(uint32_t val);

#endif // UTILS_H
