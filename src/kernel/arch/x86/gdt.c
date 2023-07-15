#include "gdt.h"
#include <string.h>
// GDT table
struct gdt_entry gdt[3];

// GDT pointer
struct gdt_ptr gp;

// Initialize a GDT entry
void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

// Initialize the GDT
void gdt_init()
{
    // Set up GDT pointer
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uint32_t)&gdt;

    // Clear GDT
    memset(&gdt, 0, sizeof(gdt));

    // Set up code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Set up data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load GDT
    _asm volatile("lgdt %0" : : "m"(gp));
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
void (*isr_table[256])(void);

// Register an ISR
void isr_register(uint8_t num, void (*handler)(void))
{
    isr_table[num] = handler;
}

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