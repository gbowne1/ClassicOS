#include "cpu.h"

// Function to read a 32-bit value from a CPU register
uint32_t read_register(uint8_t reg) {
    uint32_t value;
    __asm__ volatile("mov %0, %%" : "+r"(value) : "c"(reg));
    return value;
}

// Function to write a 32-bit value to a CPU register
void write_register(uint8_t reg, uint32_t value) {
    __asm__ volatile("mov %0, %%" : : "c"(reg), "r"(value));
}

// Function to read the value of the CR0 register
uint32_t read_cr0()
{
    uint32_t value;
    __asm__ volatile("mov %%cr0, %0" : "=r"(value));
    return value;
}

// Function to write a value to the CR0 register
void write_cr0(uint32_t value)
{
    __asm__ volatile("mov %0, %%cr0" : : "r"(value));
}

// Function to switch from real mode to protected mode
void switch_to_protected_mode()
{
    uint32_t cr0 = read_cr0();
    cr0 |= 0x1; // Set the PE bit to switch to protected mode
    write_cr0(cr0);
}

// Write a byte to a port
void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from a port
uint8_t inb(uint16_t port)
{
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Write a word to a port
void outw(uint16_t port, uint16_t value)
{
    __asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

// Read a word from a port
uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Write a double word to a port
void outl(uint16_t port, uint32_t value)
{
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

// Read a double word from a port
uint32_t inl(uint16_t port)
{
    uint32_t value;
    __asm__ volatile ("inl %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

// Execute the CPUID instruction
void cpuid(uint32_t code, uint32_t *a, uint32_t *d)
{
    __asm__ volatile ("cpuid" : "=a"(*a), "=d"(*d) : "a"(code) : "ecx", "ebx");
}
