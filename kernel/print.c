#include <stdio.h>
#include <stdarg.h>
#include "print.h"

void my_putchar(char ch) {
    // Write a single character to standard output
    // In a freestanding environment, you might need to implement this differently
    // For now, we will use the standard putchar for demonstration
    // Replace this with your own implementation if needed
    putchar(ch);
}

void print_string(const char *str) {
    // Simple implementation to print a string
    while (*str) {
        my_putchar(*str++);
    }
}

void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++; // Move to the next character after '%'
            switch (*format) {
                case 's': { // String
                    char *str = va_arg(args, char *);
                    print_string(str);
                    break;
                }
                case 'd': { // Integer
                    int num = va_arg(args, int);
                    char buffer[20]; // Buffer to hold the string representation
                    snprintf(buffer, sizeof(buffer), "%d", num);
                    print_string(buffer);
                    break;
                }
                case 'c': { // Character
                    char ch = (char)va_arg(args, int); // Promote char to int
                    my_putchar(ch);
                    break;
                }
                default:
                    my_putchar('%'); // Print the '%' if no valid format specifier
                    my_putchar(*format);
                    break;
            }
        } else {
            my_putchar(*format);
        }
        format++;
    }

    va_end(args);
}


void print_hex(unsigned int num) {
    // Buffer to hold the hexadecimal representation
    char buffer[9]; // 8 hex digits + null terminator
    buffer[8] = '\0'; // Null-terminate the string

    for (int i = 7; i >= 0; i--) {
        int digit = num & 0xF; // Get the last 4 bits
        buffer[i] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A'); // Convert to hex character
        num >>= 4; // Shift right by 4 bits
    }

    // Print the buffer, skipping leading zeros
    int leading_zero = 1;
    for (int i = 0; i < 8; i++) {
        if (buffer[i] != '0') {
            leading_zero = 0; // Found a non-zero digit
        }
        if (!leading_zero) {
            my_putchar(buffer[i]);
        }
    }
    if (leading_zero) {
        my_putchar('0'); // If all were zeros, print a single '0'
    }
}