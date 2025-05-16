#include "io.h"
#include "serial.h"
#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

void serial_init_port(uint16_t port) {
    outb(port + 1, 0x00);
    outb(port + 3, 0x80);
    outb(port + 0, 0x03);
    outb(port + 1, 0x00);
    outb(port + 3, 0x03);
    outb(port + 2, 0xC7);
    outb(port + 4, 0x0B);
}

void serial_init(void) {
    serial_init_port(COM1);
    serial_init_port(COM2);
    serial_init_port(COM3);
    serial_init_port(COM4);
}

void serial_write_char(char c) {
    while (!(inb(COM1 + 5) & 0x20));
    outb(COM1, c);
}

void serial_write(const char *str) {
    while (*str) {
        serial_write_char(*str++);
    }
}

void serial_write_string(const char* str) {
    serial_write(str);
}
