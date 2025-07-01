#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>
#include <stdarg.h> // Include for va_list and related macros

size_t my_strlen(const char *str); // Renamed to avoid conflict
int my_vsnprintf(char *str, size_t size, const char *format, ...); // Renamed to avoid conflict
int my_strcmp(const char *str1, const char *str2);

#endif // STRING_UTILS_H
