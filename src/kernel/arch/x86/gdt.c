#include "gdt.h"
#include "isr.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

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
                  uint8_t gran)
{
    gdt[num].base_low    = (base & GDT_LIMIT_MASK);
    gdt[num].base_middle = (base >> GDT_BASE_MIDDLE_SHIFT) & 0xFF;
    gdt[num].base_high   = (base >> GDT_BASE_HIGH_SHIFT) & 0xFF;
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
}

// Exception handlers
extern void divide_error();
extern void page_fault();
extern void general_protection_fault();
extern void double_fault();

// Interrupt handlers
extern void system_call();
extern void timer();
extern void keyboard();
extern void device();

// ISR table
void (*isr_table[256])(void) = {0};

// Register an ISR
void isr_register(uint8_t num, void (*handler)(struct isr_regs *regs));

// ISR handler
void isr_handler(struct isr_regs *regs)
{
    void (*handler)(void);

    handler = isr_table[regs->int_no];
    if (handler)
    {
        handler();
    }
}

// Initialize the ISR
void isr_init()
{
    // Initialize ISR table
    for (int i = 0; i < 256; i++)
    {
        isr_table[i] = NULL;
    }

    // Register exception handlers
    isr_register(0, divide_error);
    isr_register(14, page_fault);
    isr_register(13, general_protection_fault);
    isr_register(8, double_fault);

    // Register interrupt handlers
    isr_register(0x80, system_call);
    isr_register(0x20, timer);
    isr_register(0x21, keyboard);
    isr_register(0x30, device);
}