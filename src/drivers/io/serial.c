#include "serial.h"

#include <stddef.h>
#include <stdint.h>

#define PORT_BASE 0x3F8

void init_serial(uint16_t com) {
    uint16_t port = PORT_BASE + 8 * (com - 1);

    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_received(uint16_t com) {
    uint16_t port = PORT_BASE + 8 * (com - 1);

    return inb(port + 5) & 1;
}

char read_serial(uint16_t com) {
    uint16_t port = PORT_BASE + 8 * (com - 1);

    while (serial_received(com) == 0);

    return inb(port);
}

int is_transmit_empty(uint16_t com) {
    uint16_t port = PORT_BASE + 8 * (com - 1);

    return inb(port + 5) & 0x20;
}

void write_serial(uint16_t com, char a) {
    uint16_t port = PORT_BASE + 8 * (com - 1);

    while (is_transmit_empty(com) == 0);

    outb(port, a);
}