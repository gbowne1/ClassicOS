#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// Structure for storing register values during an ISR
struct isr_regs
{
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx,
        eax; // Pushed by pusha instruction
    uint32_t int_no,
        err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp,
        ss; // Pushed by the processor automatically
};

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
void page_fault(struct isr_regs *regs);
void general_protection_fault(struct isr_regs *regs);
void double_fault();

// Interrupt handlers
void system_call(struct isr_regs *regs);
void timer();
void keyboard();
void device();
void network();
void disk();
void serial_port();

// Initialize the IDT
void idt_init();

#endif /* IDT_H */