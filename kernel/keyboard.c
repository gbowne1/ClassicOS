#include "keyboard.h"
#include "io.h"
#include "isr.h"
#include "terminal.h"

#define KEYBOARD_DATA_PORT 0x60

static char key_buffer[256];
static uint8_t buffer_index = 0;

// Basic US QWERTY keymap (scancode to ASCII)
static const char scancode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', // 0x00 - 0x09
   '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', // 0x0A - 0x13
   't', 'y', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n', // 0x14 - 0x1D
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', // 0x1E - 0x27
   ';', '\'', '`', 0, '\\', 'x', 'c', 'v', 'b', // 0x28 - 0x31
   'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,      // 0x32 - 0x3B
    // rest can be filled as needed
};

// Interrupt handler for IRQ1
void keyboard_callback(void) {
    uint8_t scancode = inb(0x60);

    // Only handle key press (ignore key release)
    if (!(scancode & 0x80)) {
        char c = scancode_map[scancode];
        if (c && buffer_index < sizeof(key_buffer) - 1) {
            key_buffer[buffer_index++] = c;
            terminal_putchar(c);
        }
    }

    // Send End of Interrupt (EOI) to the PIC
    outb(0x20, 0x20);
}


void keyboard_init() {
    register_interrupt_handler(33, keyboard_callback); // IRQ1 = int 33 (0x21)
}

// Blocking read (returns one char)
char keyboard_get_char() {
    while (buffer_index == 0); // Busy wait

    char c;
    __asm__ __volatile__("cli");
    c = key_buffer[0];
    for (uint8_t i = 1; i < buffer_index; i++) {
        key_buffer[i - 1] = key_buffer[i];
    }
    buffer_index--;
    __asm__ __volatile__("sti");

    return c;
}

