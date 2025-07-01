#include "string_utils.h"
#include <stddef.h>
#include <stdarg.h>

size_t my_strlen(const char *str) {
    const char *s = str;
    while (*s) s++;
    return s - str;
}

// Forward declaration of my_itoa
static size_t my_itoa(int value, char *str, size_t size);

int my_vsnprintf(char *str, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    size_t written = 0; // Change to size_t

    for (const char *p = format; *p != '\0' && written < size - 1; p++) {
        if (*p == '%') {
            p++;
            if (*p == 's') {
                const char *s = va_arg(args, const char *);
                while (*s && written < size - 1) {
                    str[written++] = *s++;
                }
            } else if (*p == 'd') {
                // Handle integer formatting
                written += my_itoa(va_arg(args, int), str + written, size - written);
            } else {
                // Handle other formats as needed
                str[written++] = *p; // Just copy the character
            }
        } else {
            str[written++] = *p;
        }
    }
    str[written] = '\0'; // Null-terminate the string
    va_end(args);
    return written;
}

static size_t my_itoa(int value, char *str, size_t size) {
    size_t written = 0; // Change to size_t
    if (value < 0) {
        if (written < size - 1) {
            str[written++] = '-';
        }
        value = -value;
    }
    // Convert integer to string
    int temp = value;
    int digits = 0;
    do {
        digits++;
        temp /= 10;
    } while (temp);
    
    if (written + digits >= size) {
        digits = size - written - 1; // Prevent overflow
    }
    
    str += written + digits; // Move pointer to the end
    *str-- = '\0'; // Null-terminate the string

    do {
        *str-- = (value % 10) + '0';
        value /= 10;
    } while (value && str >= str - digits);
    
    return written + digits; // Return total written characters
}

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}