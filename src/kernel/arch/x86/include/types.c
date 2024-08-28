#include "types.h"

// Utility function to set a GDT entry
void gdt_set_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->limit_low = limit & 0xFFFF;
    entry->base_low = base & 0xFFFF;
    entry->base_middle = (base >> 16) & 0xFF;
    entry->access = access;
    entry->limit_high = (limit >> 16) & 0x0F;
    entry->flags = flags & 0x0F;
    entry->base_high = (base >> 24) & 0xFF;
}

// Utility function to set an IDT entry
void idt_set_entry(idt_entry_t *entry, uint32_t base, uint16_t selector, uint16_t flags) {
    entry->offset_low = base & 0xFFFF;
    entry->segment = selector;
    entry->reserved = 0;
    entry->flags = flags;
    entry->offset_high = (base >> 16) & 0xFFFF;
}
