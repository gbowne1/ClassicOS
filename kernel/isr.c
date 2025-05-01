#include "terminal.h"
#include "serial.h"

void isr_handler(uint32_t int_num, uint32_t err_code) {
    terminal_write("Interrupt occurred: ");
    // Add simple int-to-string printing here
    serial_write("INT triggered\n");

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
