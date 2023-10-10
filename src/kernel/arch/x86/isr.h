#ifndef ISR_H
#define ISR_H

#include <stdint.h>

extern void (*isr_table[256])(struct isr_regs *regs);

struct isr_regs
{
    uint32_t gs, fs, es, ds;     // Segment selectors
    uint32_t edi, esi, ebp, esp; // Pushed by pusha instruction
    uint32_t ebx, edx, ecx, eax; // Pushed by the interrupt handler
    uint32_t int_no,
        err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags,
        esp_at_signal; // Pushed by the processor automatically
};

// Structure for storing register values during an ISR
struct idt_regs
{
    uint32_t ds; // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx,
        eax; // Pushed by pusha instruction
    uint32_t int_no,
        err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp,
        ss; // Pushed by the processor automatically
};

// ISR handler
void isr_handler(struct idt_regs *regs);

#endif /* ISR_H */