#include "idt.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    GDT_ACCESS_PRESENT = 0x80,
    BASE_MIDDLE_SHIFT  = 16,
    BYTE_MASK          = 0xFF
};

// IDT table
struct idt_entry idt[256];

// IDT pointer
const struct idt_ptr idtp = {sizeof(idt) - 1, idt};

// Exception handlers
extern void divide_error_handler();
extern void page_fault_handler();
extern void general_protection_fault_handler();
extern void double_fault_handler();

// Interrupt handlers
extern void system_call_handler();
extern void timer_handler();
extern void keyboard_handler();
extern void device_handler();
extern void network_handler();
extern void disk_handler();
extern void serial_port_handler();

// Initialize an IDT entry
void idt_set_gate(uint8_t num, void *base, uint16_t sel, uint8_t flags)
{
    uintptr_t base_addr = (uintptr_t)base;
    idt[num].base_lo   = base_addr & BYTE_MASK;
    idt[num].base_hi   = (base_addr >> BASE_MIDDLE_SHIFT) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// Initialize the IDT
void idt_init()
{
    // Clear IDT
#ifdef __STDC_LIB_EXT1__
    memset_s(idt, 0, sizeof(idt));
#else
    memset(idt, 0, sizeof(idt));
#endif

    // Set up exception handlers
    idt_set_gate(0, divide_error_handler, 0x08, 0x8E);
    idt_set_gate(14, page_fault_handler, 0x08, 0x8E);
    idt_set_gate(13, general_protection_fault_handler, 0x08, 0x8E);
    idt_set_gate(8, double_fault_handler, 0x08, 0x8E);

    // Set up interrupt handlers
    idt_set_gate(0x80, system_call_handler, 0x08, 0xEE);
    idt_set_gate(0x20, timer_handler, 0x08, 0x8E);
    idt_set_gate(0x21, keyboard_handler, 0x08, 0x8E);
    idt_set_gate(0x22, network_handler, 0x08, 0x8E);
    idt_set_gate(0x23, disk_handler, 0x08, 0x8E);
    idt_set_gate(0x24, serial_port_handler, 0x08, 0x8E);

    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idtp));
}