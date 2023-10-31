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

#define BYTE_MASK    0xFF
#define PAGE_FAULT   14
#define DOUBLE_FAULT 8
#define SYSTEM_CALL  80
#define TIMER        20

#define ISR_TABLE_SIZE 256

// GDT table
struct gdt_entry *gdt;

// GDT constants
enum GDT_ACCESS
{
    GDT_ACCESS_PRESENT = 0x80
};

// GDT base and limit constants
enum GDT_BASE_LIMIT
{
    GDT_BASE_MIDDLE_SHIFT = 16,
    GDT_BASE_HIGH_SHIFT   = 24,
    GDT_GRANULARITY_SHIFT = 16,
    GDT_GRANULARITY_MASK  = 0x0F,
    GDT_ACCESS_MASK       = 0xF0,
    GDT_LIMIT_MASK        = 0xFFFF
};

// Initialize a GDT entry
void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran, struct gdt_entry *const gdt)
{
    gdt[num].base_low    = (base & GDT_LIMIT_MASK);
    gdt[num].base_middle = (base >> GDT_BASE_MIDDLE_SHIFT) & BYTE_MASK;
    gdt[num].base_high   = (base >> GDT_BASE_HIGH_SHIFT) & BYTE_MASK;
    gdt[num].limit_low   = (limit & GDT_LIMIT_MASK);
    gdt[num].granularity =
        (limit >> GDT_GRANULARITY_SHIFT) & GDT_GRANULARITY_MASK;
    gdt[num].granularity |= gran & GDT_ACCESS_MASK;
    gdt[num].access = access;
}

void gdt_init()
{
    // Set up GDT pointer
    struct gdt_ptr gp;
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gdt      = (struct gdt_entry *)malloc(sizeof(struct gdt_entry) * 3);
    memset(gdt, 0, sizeof(struct gdt_entry) * 3);

    // Initialize GDT entries
    gdt_set_gate(0, 0, 0, 0, 0, gdt);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF, gdt); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF, gdt); // Data segment

    // Load GDT
    struct gdt_ptr gdtp;
    gdtp.limit = gp.limit;
    gdtp.base  = (uintptr_t)gdt;
    __asm__ volatile("lgdt %0" : : "m"(gdtp));
    __asm__ volatile("mov $0x10, %ax\n\t"
                     "mov %ax, %ds\n\t"
                     "mov %ax, %es\n\t"
                     "mov %ax, %fs\n\t"
                     "mov %ax, %gs\n\t"
                     "ljmp $0x08, $next_label\n\t"
                     "next_label:");
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