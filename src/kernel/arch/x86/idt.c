#include "idt.h"

// IDT table
struct idt_entry idt[256];

// IDT pointer
struct idt_ptr idtp;

// Exception handlers
extern void divide_error_handler();
extern void page_fault_handler();
extern void general_protection_fault_handler();
extern void double_fault_handler();

// Interrupt handlers
extern void system_call_handler();
extern void timer_handler();
extern void keyboard_handler();
extern void device_handler();

// Initialize an IDT entry
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel     = sel;
    idt[num].always0 = 0;
    idt[num].flags   = flags;
}

// Initialize the IDT
void idt_init()
{
    // Set up IDT pointer
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    // Clear IDT
    memset(&idt, 0, sizeof(idt));

    // Set up exception handlers
    idt_set_gate(0, (uint32_t)divide_error_handler, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)page_fault_handler, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)general_protection_fault_handler, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)double_fault_handler, 0x08, 0x8E);

    // Set up interrupt handlers
    idt_set_gate(0x80, (uint32_t)system_call_handler, 0x08, 0xEE);
    idt_set_gate(0x20, (uint32_t)timer_handler, 0x08, 0x8E);
    idt_set_gate(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);
    idt_set_gate(0x30, (uint32_t)device_handler, 0x08, 0x8E);

    // Load IDT
    asm volatile("lidt %0" : : "m"(idtp));
}