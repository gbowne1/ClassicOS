#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Function to read a 32-bit value from a CPU register
uint32_t read_register(uint8_t reg);

// Function to write a 32-bit value to a CPU register
void write_register(uint8_t reg, uint32_t value);

// Function to read the value of the CR0 register
uint32_t read_cr0();

// Function to write a value to the CR0 register
void write_cr0(uint32_t value);

// Function to switch from real mode to protected mode
void switch_to_protected_mode();

void    outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

void     outw(uint16_t port, uint16_t value);
uint16_t inw(uint16_t port);

void     outl(uint16_t port, uint32_t value);
uint32_t inl(uint16_t port);

void cpuid(uint32_t code, uint32_t *a, uint32_t *d);

#endif