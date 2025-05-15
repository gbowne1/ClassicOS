#include "utils.h"

static void reverse(char* str, int len) {
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start++] = str[end];
        str[end--] = temp;
    }
}

// Integer to ASCII for signed ints
char* itoa(int value, char* str, int base) {
    int i = 0;
    int isNegative = 0;
    unsigned int uval;

    if (base < 2 || base > 36) {
        str[0] = '\0';
        return str;
    }

    // Handle zero explicitly
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers (only for base 10)
    if (value < 0 && base == 10) {
        isNegative = 1;
        uval = (unsigned int)(-value);
    } else {
        uval = (unsigned int)value;
    }

    while (uval != 0) {
        int rem = uval % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        uval /= base;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

// Integer to ASCII for unsigned ints
char* utoa(unsigned int value, char* str, int base) {
    int i = 0;

    if (base < 2 || base > 36) {
        str[0] = '\0';
        return str;
    }

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value /= base;
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

void print_hex(uint32_t val) {
    char hex_chars[] = "0123456789ABCDEF";
    char buf[11] = "0x00000000";
    for (int i = 9; i >= 2; i--) {
        buf[i] = hex_chars[val & 0xF];
        val >>= 4;
    }
    terminal_write(buf);
    serial_write(buf);
}