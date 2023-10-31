#ifndef ISR_H
#define ISR_H

#include "../include/types.h"

enum ISR_Vector : uint32_t
{
	EXCEPTION_START					   = 0x00,
	DIVIDE_BY_ZERO_INTERRUPT		   = 0x00,
	DOUBLE_FAULT_INTERRUPT			   = 0x08,
	PAGE_FAULT_INTERRUPT			   = 0x0E,
	GENERAL_PROTECTION_FAULT_INTERRUPT = 0x0D,
	EXCEPTION_END					   = 0x1F,
	HARDWARE_START					   = 0x20,
	TIMER_INTERRUPT					   = 0x20,
	KEYBOARD_INTERRUPT				   = 0x21,
	NETWORK_INTERRUPT				   = 0x22,
	DISK_INTERRUPT					   = 0x23,
	SERIAL_PORT_INTERRUPT			   = 0x24,
	HARDWARE_END					   = 0x2F,
	USER_DEFINED_START				   = 0x30,
	SYSTEM_CALL_INTERRUPT			   = 0x80
};

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

#endif /* ISR_H */