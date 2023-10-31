#ifndef IDT_H
#define IDT_H

#include "include/types.h"

// IDT entry structure
struct idt_entry
{
    uint16_t base_lo; // Lower 16 bits of handler function address
    uint16_t sel;     // Kernel segment selector
    uint8_t  always0; // Always 0
    uint8_t  flags;   // Flags
    uint16_t base_hi; // Upper 16 bits of handler function address
} __attribute__((packed));

extern struct idt_entry idt[256];

// Initialize the IDT
void InitializeIDT();

#endif /* IDT_H */