#ifndef IO_H
#define IO_H

#include <stdint.h>

// Function to initialize the COM and LPT ports
void io_init();

// Function to read from the COM port
char io_read_com();

// Function to write to the COM port
void io_write_com(char data);

// Function to read from the LPT port
char io_read_lpt();

// Function to write to the LPT port
void io_write_lpt(char data);

// Function declarations for keyboard.c
extern uint8_t inb(uint16_t port);

extern void outb(uint16_t port, uint8_t data);

void    install_interrupt_handler(uint8_t interrupt, void (*handler)(void));

#endif /* IO_H */