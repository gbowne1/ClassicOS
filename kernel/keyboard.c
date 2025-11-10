#include "keyboard.h"
#include "io.h"
#include "isr.h"
#include "terminal.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEY_BUFFER_SIZE 256

static char key_buffer[KEY_BUFFER_SIZE];
static uint8_t buffer_head = 0;  // Write position (interrupt)
static uint8_t buffer_tail = 0;  // Read position (get_char)
static uint8_t buffer_count = 0;
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
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) return;  // Ignore key release

    char c = scancode_map[scancode];
    if (!c) return;

    uint8_t next_head = (buffer_head + 1) % KEY_BUFFER_SIZE;

    // Drop key if buffer full
    if (next_head == buffer_tail) return;

    key_buffer[buffer_head] = c;
    buffer_head = next_head;
    buffer_count++;

    terminal_putchar(c);
}

void keyboard_init() {
    register_interrupt_handler(33, keyboard_callback); // IRQ1 = int 33 (0x21)
}

// Blocking read (returns one char)
char keyboard_get_char(void) {
    while (buffer_count == 0) {
        __asm__ __volatile__("hlt");  // Better than busy loop
    }

    char c;
    __asm__ __volatile__("cli");
    c = key_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEY_BUFFER_SIZE;
    buffer_count--;
    __asm__ __volatile__("sti");

    return c;
}
