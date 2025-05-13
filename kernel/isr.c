#include "terminal.h"
#include "serial.h"
#include "isr.h"

static isr_callback_t interrupt_handlers[MAX_INTERRUPTS] = { 0 };

void isr_handler(uint32_t int_num, uint32_t err_code) {
    terminal_write("Interrupt occurred: ");
    // Here you can add a basic itoa to print int_num
    serial_write("INT triggered\n");

    if (interrupt_handlers[int_num]) {
        interrupt_handlers[int_num]();  // Call registered handler
    } else {
        terminal_write(" -> No handler registered\n");

        if (int_num == 0) {
            terminal_write(" -> Divide by zero error!\n");
        } else if (int_num == 13) {
            terminal_write(" -> General Protection Fault!\n");
        } else {
            terminal_write(" -> Unknown interrupt\n");
        }

        // Halt CPU
        while (1) {
            asm volatile ("hlt");
        }
    }
}

void register_interrupt_handler(uint8_t n, isr_callback_t handler) {
    interrupt_handlers[n] = handler;
}
