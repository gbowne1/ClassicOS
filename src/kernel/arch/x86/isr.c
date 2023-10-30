#include "isr.h"
#include "idt.h"
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "./interrupt_utils.h"

void invalid_opcode(void);
void general_protection_fault(struct idt_regs *regs);
void page_fault(struct idt_regs *regs);
void system_call(struct idt_regs *regs);

enum
{
    TIMER_INTERRUPT       = 0x20,
    KEYBOARD_INTERRUPT    = 0x21,
    NETWORK_INTERRUPT     = 0x22,
    DISK_INTERRUPT        = 0x23,
    SERIAL_PORT_INTERRUPT = 0x24
};

void dummy_isr(struct isr_regs *regs)
{
    printf("Timer interrupt occurred!\n");
}

// Register an ISR
extern void isr_register(uint8_t num, void (*handler)(struct isr_regs *regs))
{
    isr_table[num] = handler;
}

// ISR handler
void isr_handler(struct idt_regs *regs)
{
    switch (regs->int_no)
    {
    case 0x00:
        divide_error(regs);
        break;
    case 0x06:
        invalid_opcode();
        break;
    case 0x0D:
        general_protection_fault(regs);
        break;
    case 0x0E:
        page_fault(regs);
        break;
    case 0x08:
        double_fault();
        break;
    case 0x80:
        system_call(regs);
        break;
    case 0x20:
        timer();
        break;
    case 0x21:
        keyboard();
        break;
    case 0x2F:
        device();
        break;
    case 0x28:
        network();
        break;
    case 0x2E:
        disk();
        break;
    case 0x24:
        serial_port();
        break;
    default:
        printf("Unhandled interrupt: %d\n", regs->int_no);
        break;
    }
}

// Exception handlers
void divide_error(struct idt_regs *regs)
{
    printf("Divide by zero error!\n");
    // Additional actions can be taken as needed
}

void page_fault(struct idt_regs *regs)
{
    uint32_t faulting_address;

    // Read the CR2 register to get the faulting address
    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

    // Print an error message with the faulting address
    printf("Page fault at 0x%x, virtual address 0x%x\n", regs->eip,
           faulting_address);

    // Additional actions can be taken as needed
}

void general_protection_fault(struct idt_regs *regs)
{
    printf("General protection fault occurred!\n");
    // Additional actions can be taken as needed
}

void double_fault()
{
    // Handle double fault exception
}

// Interrupt handlers
void system_call(struct idt_regs *regs)
{
    // Get the system call number from the eax register
    uint32_t syscall_number = regs->eax;

    // Handle different system call numbers
    switch (syscall_number)
    {
    case 1:
        // Handle open() system call
        // ...
        break;
    case 2:
        // Handle read() system call
        // ...
        break;
    case 3:
        // Handle write() system call
        // ...
        break;
    case 4:
        // Handle close() system call
        // ...
        break;
        // Add more cases for other system calls

    default:
        // Unknown system call number
        printf("Unknown system call number: %d\n", syscall_number);
        break;
    }
}

void timer()
{
	static int count = 0;
	printf ("timer expired %d times\n", ++count);
}

void keyboard()
{
    // Handle keyboard interrupt
}

void device()
{
    // Determine the type of device interrupt
    uint32_t interrupt_type = read_interrupt_type();

    // Call the appropriate interrupt handler
    switch (interrupt_type)
    {
    case TIMER_INTERRUPT:
        timer();
        break;
    case KEYBOARD_INTERRUPT:
        keyboard();
        break;
    case NETWORK_INTERRUPT:
        network();
        break;
    case DISK_INTERRUPT:
        disk();
        break;
    case SERIAL_PORT_INTERRUPT:
        serial_port();
        break;
        // Add more cases for other types of device interrupts

    default:
        // Unknown interrupt type
        printf("Unknown device interrupt type: %d\n", interrupt_type);
        break;
    }
}
