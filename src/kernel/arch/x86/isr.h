#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Structure for storing register values during an ISR
struct isr_regs
{
    uint32_t ds;  // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx,
        eax;      // Pushed by pusha instruction
    uint32_t int_no,
        err_code; // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp,
        ss;       // Pushed by the processor automatically
};

// Register an ISR
void isr_register(uint8_t num, void (*handler)(struct isr_regs *regs));

// ISR handler
void isr_handler(struct isr_regs *regs);

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

#endif