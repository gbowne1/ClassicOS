#include "idt.h"
#include "../../../drivers/keyboard/keyboard.h"
#include "isr/isr.h"
#include <stdint.h>

struct idt_entry idt[256];

extern void LoadIDT(struct idt_entry *entry);

void TimerInterruptHandler()
{
    static uint16_t timer_count = 0;
    timer_count++;
}

// Initialize the IDT
void InitializeIDT()
{
	idt[KEYBOARD_INTERRUPT].base_lo = ((uint16_t *)KeyboardInterruptHandler)[0];
	idt[KEYBOARD_INTERRUPT].sel = 0x08;
	idt[KEYBOARD_INTERRUPT].always0 = 0x00;
	idt[KEYBOARD_INTERRUPT].flags = 0x8E;
	idt[KEYBOARD_INTERRUPT].base_hi = ((uint16_t *)KeyboardInterruptHandler)[1];

	idt[0x33].base_lo = (uint16_t)TimerInterruptHandler;
	idt[0x33].sel = 0x08;
	idt[0x33].always0 = 0x00;
	idt[0x33].flags = 0x8E;
	idt[0x33].base_hi = (uint16_t)TimerInterruptHandler >> 16;

	LoadIDT(&idt[0]);
}
