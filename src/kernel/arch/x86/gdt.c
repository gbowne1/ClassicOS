#include "gdt.h"
#include "idt.h"
#include "isr/isr.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#define BYTE_MASK 0xFF
#define PAGE_FAULT 14
#define DOUBLE_FAULT 8
#define SYSTEM_CALL 80
#define TIMER 20

#define ISR_TABLE_SIZE 256

// GDT table
struct gdt_entry gdt_entries[3] __attribute__((aligned(0x1000)));

// GDT constants
enum GDT_ACCESS
{
	GDT_ACCESS_PRESENT = 0x80
};

// GDT base and limit constants
enum GDT_BASE_LIMIT
{
	GDT_BASE_MIDDLE_SHIFT = 16,
	GDT_BASE_HIGH_SHIFT = 24,
	GDT_GRANULARITY_SHIFT = 16,
	GDT_GRANULARITY_MASK = 0x0F,
	GDT_ACCESS_MASK = 0xF0,
	GDT_LIMIT_MASK = 0xFFFF
};

// Initialize a GDT entry
void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
				  uint8_t gran, struct gdt_entry *const gdt)
{
	gdt[num].base_low = (base & GDT_LIMIT_MASK);
	gdt[num].base_middle = (base >> GDT_BASE_MIDDLE_SHIFT) & BYTE_MASK;
	gdt[num].base_high = (base >> GDT_BASE_HIGH_SHIFT) & BYTE_MASK;
	gdt[num].limit_low = (limit & GDT_LIMIT_MASK);
	gdt[num].granularity =
		(limit >> GDT_GRANULARITY_SHIFT) & GDT_GRANULARITY_MASK;
	gdt[num].granularity |= gran & GDT_ACCESS_MASK;
	gdt[num].access = access;
}

extern bool gdt_init(void)
{
	// Set up GDT pointer
	struct gdt_ptr gp;
	gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gp.base = (uintptr_t)&gdt_entries[0];

	// Initialize GDT entries
	gdt_set_gate(0, 0, 0, 0, 0, &gdt_entries[0]);				 // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF, &gdt_entries[1]); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF, &gdt_entries[2]); // Data segment

	// Load GDT
	LoadGDT(&gp);

	// Switch to protected mode
	__asm__ volatile(
		"movw $0x10, %%ax\n\t"
		"movw %%ax, %%ds\n\t"
		"movw %%ax, %%es\n\t"
		"movw %%ax, %%fs\n\t"
		"movw %%ax, %%gs\n\t"
		"movw %%ax, %%ss\n\t"
		"ljmp $0x08, $1f\n\t"
		"1:\n\t"
		:
		:
		: "ax");

	return true; // Indicate successful GDT initialization
}

// Exception handlers
extern void divide_error(struct idt_regs *regs);
extern void page_fault(struct idt_regs *regs);
extern void general_protection_fault();
extern void double_fault();

// Interrupt handlers
extern void system_call();
extern void timer();
extern void keyboard();
extern void device();
