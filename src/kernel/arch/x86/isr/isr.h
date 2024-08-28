#ifndef ISR_H
#define ISR_H

#include "../include/types.h"

enum ISR_Vector
{
	EXCEPTION_START = 0x00,
	DIVIDE_BY_ZERO_INTERRUPT = 0x0,
	DOUBLE_FAULT_INTERRUPT = 0x8,
	PAGE_FAULT_INTERRUPT = 0xE,
	GENERAL_PROTECTION_FAULT_INTERRUPT = 0xD,
	EXCEPTION_END = 0x1F,
	HARDWARE_START = 0x20,
	TIMER_INTERRUPT = 0x20,
	KEYBOARD_INTERRUPT = 0x21,
	NETWORK_INTERRUPT = 0x22,
	DISK_INTERRUPT = 0x23,
	SERIAL_PORT_INTERRUPT = 0x24,
	HARDWARE_END = 0x2F,
	USER_DEFINED_START = 0x30,
	SYSTEM_CALL_INTERRUPT = 0x80
};

struct isr_regs
{
	uint32_t gs, fs, es, ds;				 // Segment selectors
	uint32_t edi, esi, ebp, esp;			 // Pushed by pusha instruction
	uint32_t ebx, edx, ecx, eax;			 // Pushed by the interrupt handler
	uint32_t int_no, err_code;				 // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, esp_at_signal; // Pushed by the processor automatically
};

void clear_interrupt(uint32_t interrupt_id)
{
	// Assuming interrupt_id maps to a specific bit in an interrupt control register
	// For example, if DIVIDE_BY_ZERO_INTERRUPT corresponds to bit 0 in a register at address 0x12345678
	uint32_t *interrupt_control_register = (uint32_t *)0x12345678;
	*interrupt_control_register &= ~(1 << interrupt_id);
}

extern void isr_handler(struct isr_regs regs);

#endif /* ISR_H */
