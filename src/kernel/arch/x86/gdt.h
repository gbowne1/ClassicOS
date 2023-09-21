// gdt.h
#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT table
extern struct gdt_entry *gdt;

// GDT entry structure
struct gdt_entry
{
    uint16_t limit_low;         // The lower 16 bits of the limit
    uint16_t base_low;          // The lower 16 bits of the base
    uint8_t base_middle;        // The next 8 bits of the base
    uint8_t access;             // Access flags, determine what ring this segment can be used in
    uint8_t granularity;
    uint8_t base_high;          // The last 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr
{
    uint16_t limit;             // The upper 16 bits of all selector limits
    uint32_t base;              // The address of the first gdt_entry_t struct
} __attribute__((packed));

// Initialize the GDT
void gdt_init();

// Set a GDT entry
void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#endif