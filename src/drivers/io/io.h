#ifndef IO_H
#define IO_H

#include <stdint.h>

// Inline assembly implementations for I/O operations
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Function to initialize the COM and LPT ports
void io_init(uint16_t port);

// Function to read from the COM port
char io_read_com();

// Function to write to the COM port
void io_write_com(char data);

// Function to read from the LPT port
char io_read_lpt();

// Function to write to the LPT port
void io_write_lpt(char data);

// Function declaration for interrupt handler installation
void install_interrupt_handler(uint8_t interrupt, void (*handler)(void));

#endif /* IO_H */
