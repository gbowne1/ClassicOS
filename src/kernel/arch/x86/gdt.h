#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>

// GDT entry structure
struct gdt_entry
{
	uint16_t limit_low;	 // The lower 16 bits of the limit
	uint16_t base_low;	 // The lower 16 bits of the base
	uint8_t base_middle; // The next 8 bits of the base
	uint8_t access;		 // Access flags, determine what ring this segment can be used in
	uint8_t granularity; // Granularity
	uint8_t base_high;	 // The last 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr
{
	uint16_t limit; // The upper 16 bits of all selector limits
	uint32_t base;	// The address of the first gdt_entry_t struct
} __attribute__((packed));

// Function prototypes
extern bool gdt_init(void);
extern bool init_gdt(void);
void LoadGDT(struct gdt_ptr *gdt); // External declaration of LoadGDT

#endif // GDT_H
