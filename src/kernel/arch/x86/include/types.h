#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// Basic integer types
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;

// Attribute for packed structures
#define PACKED_ATTRIBUTE __attribute__((packed))

// Structure for GDT entries
typedef struct {
    uint16_t limit_low;   // Limit lower 16 bits
    uint16_t base_low;    // Base address lower 16 bits
    uint8_t  base_middle; // Base address middle 8 bits
    uint8_t  access;      // Access flags
    uint8_t  limit_high : 4;  // Limit upper 4 bits
    uint8_t  flags : 4;       // Flags (4 bits)
    uint8_t  base_high;   // Base address upper 8 bits
} PACKED_ATTRIBUTE gdt_entry_t;

// Structure for IDT entries
typedef struct {
    uint16_t offset_low;  // Offset lower 16 bits
    uint16_t segment;     // Segment selector
    uint8_t  reserved;    // Reserved
    uint8_t  flags;       // Flags
    uint16_t offset_high; // Offset upper 16 bits
} PACKED_ATTRIBUTE idt_entry_t;

#endif // TYPES_H