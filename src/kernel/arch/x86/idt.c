#include "idt.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IDT table
struct idt_entry idt[256];

// IDT pointer
struct idt_ptr idtp;

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

// Initialize an IDT entry
void idt_set_gate(uint8_t num, void *base, uint16_t sel, uint8_t flags)
{
    uint32_t base_addr = (uint32_t)base;
    idt[num].base_lo   = base_addr & 0xFFFF;
    idt[num].base_hi   = (base_addr >> 16) & 0xFFFF;
    idt[num].sel       = sel;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

// Initialize the IDT
void idt_init()
{
    // Set up IDT pointer
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = &idt[0];

    // Clear IDT
    memset(&idt, 0, sizeof(idt));

    // Set up exception handlers
    idt_set_gate(0, divide_error_handler, 0x08, 0x8E);
    idt_set_gate(14, page_fault_handler, 0x08, 0x8E);
    idt_set_gate(13, general_protection_fault_handler, 0x08, 0x8E);
    idt_set_gate(8, double_fault_handler, 0x08, 0x8E);

    // Set up interrupt handlers
    idt_set_gate(0x80, system_call_handler, 0x08, 0xEE);
    idt_set_gate(0x20, timer_handler, 0x08, 0x8E);
    idt_set_gate(0x21, keyboard_handler, 0x08, 0x8E);
    idt_set_gate(0x30, device_handler, 0x08, 0x8E);

    // Load IDT
    __asm__ volatile("lidt %0" : : "m"(idtp));
}