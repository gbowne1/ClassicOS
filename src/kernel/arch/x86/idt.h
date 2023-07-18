#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT entry structure
struct idt_entry
{
    uint16_t base_lo; // Lower 16 bits of handler function address
    uint16_t sel;     // Kernel segment selector
    uint8_t  always0; // Always 0
    uint8_t  flags;   // Flags
    uint16_t base_hi; // Upper 16 bits of handler function address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr
{
    uint16_t          limit; // Size of IDT in bytes - 1
    struct idt_entry *base;  // Address of IDT
} __attribute__((packed));

// Exception handlers
void divide_error();
void page_fault();
void general_protection_fault();
void double_fault();

// Interrupt handlers
void system_call();
void timer();
void keyboard();
void device();

// Initialize the IDT
void idt_init();

#endif /* IDT_H */