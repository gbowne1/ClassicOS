#include "terminal.h"
#include "serial.h"
#include "isr.h"
#include "io.h"
#include "utils.h"

static isr_callback_t interrupt_handlers[MAX_INTERRUPTS] = { 0 };

void isr_handler(uint32_t int_num, uint32_t err_code) {
    terminal_write("Interrupt occurred: ");

    print_hex(int_num);
    terminal_write("\n");

    serial_write("INT triggered\n");

    terminal_write("Error code: ");
    print_hex(err_code);
    terminal_write("\n");

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

    // === Send End Of Interrupt to PIC(s) ===
    if (int_num >= 40) {
        // Send reset signal to slave PIC
        outb(0xA0, 0x20);
    }
    if (int_num >= 32) {
        // Send reset signal to master PIC
        outb(0x20, 0x20);
    }
}

void register_interrupt_handler(uint8_t n, isr_callback_t handler) {
    interrupt_handlers[n] = handler;
}
