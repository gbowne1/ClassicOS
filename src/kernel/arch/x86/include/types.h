#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct {
    uint32_t base;
    uint32_t limit;
    uint16_t flags;
} gdt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} idt_entry_t;

#endif /* TYPES_H */