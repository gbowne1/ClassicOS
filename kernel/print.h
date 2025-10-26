#ifndef PRINT_H
#define PRINT_H

#include "types.h"

void print_string(const char *str);
void my_printf(const char *format, ...);
void print_hex(uint32_t val, int include_prefix, int suppress_leading_zeros);
void my_putchar(char ch);

#endif
