#include "idt.h"
#include "isr/isr.h"
#include "../../../drivers/keyboard/keyboard.h"

struct idt_entry idt[256];

extern void LoadIDT(struct idt_entry* entry);

// Initialize the IDT
void InitializeIDT()
{
	idt[KEYBOARD_INTERRUPT].base_lo = ((uint16_t*)KeyboardInterruptHandler)[0];
	idt[KEYBOARD_INTERRUPT].sel = 0x08;
	idt[KEYBOARD_INTERRUPT].always0 = 0x00;
	idt[KEYBOARD_INTERRUPT].flags = 0x8E;
	idt[KEYBOARD_INTERRUPT].base_hi = ((uint16_t*)KeyboardInterruptHandler)[1];

	LoadIDT(&idt[KEYBOARD_INTERRUPT]);
}