#include "idt.h"
#include "io.h"

#define KERNEL_CS 0x08  // Kernel code segment selector

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

// External assembly stubs for ISRs (provided below)
extern void isr0();
extern void isr13();
extern void isr_default();

// Helper to set an IDT gate
void idt_set_gate(int n, uint32_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E;  // Present, ring 0, 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

// Load IDT via lidt
static void idt_load() {
    asm volatile("lidt (%0)" : : "r" (&idt_ptr));
}

// IDT initialization
void idt_init() {
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t)&idt;

    // Clear all entries
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, (uint32_t)isr_default);
    }

    // Set specific handlers
    idt_set_gate(0, (uint32_t)isr0);   // Divide by zero
    idt_set_gate(13, (uint32_t)isr13); // General protection fault

    idt_load();
}
