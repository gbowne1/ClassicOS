#include "timer.h"
#include "io.h"
#include "isr.h"
#include "terminal.h"
#include "stdio.h"

static volatile uint32_t tick = 0;

void timer_callback(void) {
    tick++;

    if (tick % 100 == 0) {
        char buf[16];
        itoa(tick, buf, 10);
        terminal_write("Tick count: ");
        terminal_write(buf);
        terminal_write("\n");
    }

    outb(0x20, 0x20); // EOI to PIC
}

void timer_init(uint32_t frequency) {
    register_interrupt_handler(32, timer_callback); // IRQ0 = Interrupt 32

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);             // Command byte
    outb(0x40, divisor & 0xFF);   // Low byte
    outb(0x40, (divisor >> 8));   // High byte
}

uint32_t timer_get_ticks(void) {
    return tick;
}
