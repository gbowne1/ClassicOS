#include "panic.h"
#include "terminal.h"
#include "serial.h"
#include <stdbool.h>

void panic(const char *message) {
    terminal_write("KERNEL PANIC: ");
    terminal_write(message);
    terminal_write("\nSystem halted.\n");

    serial_write("KERNEL PANIC: ");
    serial_write(message);
    serial_write("\nSystem halted.\n");

    // Halt the system
    while (true) {
        __asm__("cli; hlt");
    }
}
