#include <stdarg.h>
#include "print.h"
#include "serial.h"
#include "terminal.h"

void my_putchar(char ch) {
    // Write a single character to standard output
    // In a freestanding environment, you might need to implement this differently
    // For now, we will use the standard putchar for demonstration
    // Replace this with your own implementation if needed
    terminal_putchar(ch);
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
                    
                    //TODO: implement `snprintf()`
                    //snprintf(buffer, sizeof(buffer), "%d", num);
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

void print_hex(uint32_t val, int include_prefix, int suppress_leading_zeros) { 
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[11]; // 8 hex digits + "0x" + null terminator
    int pos = 10;    // Start from end of buffer (null terminator)

    // Null-terminate the buffer
    buffer[pos--] = '\0';

    // Convert value to hex digits
    for (int i = 7; i >= 0; i--) {
        int digit = val & 0xF; // Get last 4 bits
        buffer[pos--] = hex_chars[digit];
        val >>= 4; // Shift right by 4 bits
    }

    // Add "0x" prefix if requested
    if (include_prefix) {
        buffer[pos--] = 'x';
        buffer[pos--] = '0';
    }

    // Determine start of output (skip leading zeros if requested)
    int start = include_prefix ? 0 : 2; // Start after "0x" if prefix included
    if (suppress_leading_zeros && !include_prefix) {
        int i = start;
        while (i < 9 && buffer[i] == '0') {
            i++;
        }
        if (i == 10) {
            // All zeros, output single '0'
            terminal_write("0");
            serial_write("0");
            return;
        }
        start = i;
    }

    // Output the result
    terminal_write(buffer + start);
    serial_write(buffer + start);
}
