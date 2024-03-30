#include "./types.h"
#include <stdint.h>

void gdt_set_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit,
                   uint16_t flags)
{
    entry->base  = base;
    entry->limit = limit;
    entry->flags = flags;
}

void idt_set_entry(idt_entry_t *entry, uint32_t base, uint16_t selector,
                   uint16_t flags)
{
    entry->base_low  = base & 0xFFFF;
    entry->base_high = (base >> 16) & 0xFFFF;
    entry->selector  = selector;
    entry->flags     = flags;
}
