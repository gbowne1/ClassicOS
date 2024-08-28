#include "idt.h"
#include "../../../drivers/keyboard/keyboard.h"
#include "isr/isr.h"
#include <stdint.h>

struct idt_entry idt[256];

extern void LoadIDT(struct idt_entry *entry);

void TimerInterruptHandler()
{
    static uint16_t timer_count = 0;
    timer_count++;
}

union interrupt_handler_union {
    void (*handler)();
    struct {
        uint16_t low;
        uint16_t high;
    } parts;
};
// Initialize the IDT
bool InitializeIDT()
{
    union interrupt_handler_union keyboard_handler;
    keyboard_handler.handler = KeyboardInterruptHandler;

    idt[KEYBOARD_INTERRUPT].base_lo = keyboard_handler.parts.low;
    idt[KEYBOARD_INTERRUPT].sel = 0x08;
    idt[KEYBOARD_INTERRUPT].always0 = 0x00;
    idt[KEYBOARD_INTERRUPT].flags = 0x8E;
    idt[KEYBOARD_INTERRUPT].base_hi = keyboard_handler.parts.high;

    union interrupt_handler_union timer_handler;
    timer_handler.handler = TimerInterruptHandler;

    idt[0x33].base_lo = timer_handler.parts.low;
    idt[0x33].sel = 0x08;
    idt[0x33].always0 = 0x00;
    idt[0x33].flags = 0x8E;
    idt[0x33].base_hi = timer_handler.parts.high;

    LoadIDT(&idt[0]);

    return true;  // Assuming initialization always succeeds
}
