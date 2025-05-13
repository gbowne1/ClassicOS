#include "gdt.h"

// Structure of a GDT entry (8 bytes)
struct gdt_entry {
    uint16_t limit_low;   // Limit bits 0–15
    uint16_t base_low;    // Base bits 0–15
    uint8_t  base_middle; // Base bits 16–23
    uint8_t  access;      // Access flags
    uint8_t  granularity; // Granularity + limit bits 16–19
    uint8_t  base_high;   // Base bits 24–31
} __attribute__((packed));

// Structure of the GDT pointer
struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

// Declare GDT with 3 entries
static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

// External ASM function to load GDT
extern void gdt_flush(uint32_t);

// Set an individual GDT entry
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);

    gdt[num].access      = access;
}

// Initialize the GDT
void gdt_init(void) {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    gdt_flush((uint32_t)&gp);
}
