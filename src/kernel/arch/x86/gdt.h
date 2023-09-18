#ifndef GDT_H
#define GDT_H

#include <stdint.h>

extern struct gdt_entry *gdt;

// GDT entry structure
struct gdt_entry
{
    uint16_t limit_low;   // Lower 16 bits of segment limit
    uint16_t base_low;    // Lower 16 bits of segment base address
    uint8_t  base_middle; // Middle 8 bits of segment base address
    uint8_t  access;      // Access flags
    uint8_t  granularity; // Granularity and segment limit flags
    uint8_t  base_high;   // Upper 8 bits of segment base address
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr
{
    uint16_t limit; // Size of GDT in bytes - 1
    uint32_t base;  // Address of GDT
} __attribute__((packed));

// Initialize the GDT
void gdt_init();

#endif