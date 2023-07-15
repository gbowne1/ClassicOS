#ifndef TYPES_H
#define TYPES_H

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef struct
{
    uint32_t base;
    uint32_t limit;
    uint16_t flags;
} gdt_entry_t;

typedef struct
{
    uint16_t base_lo;  // Lower 16 bits of handler function address
    uint16_t selector; // Kernel segment selector
    uint8_t  always0;  // Always 0
    uint8_t  flags;    // Flags
    uint16_t base_hi;  // Upper 16 bits of handler function address
} idt_entry_t;

#endif /* TYPES_H */