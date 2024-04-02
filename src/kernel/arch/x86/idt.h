#ifndef IDT_H
#define IDT_H

#include "include/types.h"

#define IDT_ENTRY_SIZE 16

#if IDT_ENTRY_SIZE != 16
#error "idt_entry structure size mismatch!"
#endif

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

extern void KeyboardInterruptHandler();
extern void TimerInterruptHandler();

extern void LoadIDT(struct idt_entry *entry);

#endif /* IDT_H */
