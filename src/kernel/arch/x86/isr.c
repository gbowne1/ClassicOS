#include "isr.h"
#include "idt.h"
#include <stdio.h>

// ISR table
void (*isr_table[256])(struct isr_regs *regs);

// Register an ISR
void isr_register(uint8_t num, void (*handler)(struct isr_regs *regs))
{
    isr_table[num] = handler;
}

// ISR handler
void isr_handler(struct isr_regs *regs)
{
    if (regs->int_no == 0)
    {
        divide_error();
    }
    else
    {
        void (*handler)(struct isr_regs *regs);

        handler = isr_table[regs->int_no];
        if (handler)
        {
            handler(regs);
        }
    }
}

// Exception handlers
void divide_error()
{
    printf("Divide by zero error!\n");
}

void page_fault()
{
    // Handle page fault exception
}

void general_protection_fault()
{
    // Handle general protection fault exception
}

void double_fault()
{
    // Handle double fault exception
}

// Interrupt handlers
void system_call()
{
    // Handle system call interrupt
}

void timer()
{
    // Handle timer interrupt
}

void keyboard()
{
    // Handle keyboard interrupt
}

void device()
{
    // Handle device interrupt
}