#include "keyboard.h"
#include "io.h"
#include "isr.h"
#include "terminal.h"
#include <stddef.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEY_BUFFER_SIZE 256

// Use volatile so the compiler knows these change inside interrupts
static volatile char key_buffer[KEY_BUFFER_SIZE];
static volatile uint8_t buffer_head = 0;
static volatile uint8_t buffer_tail = 0;
static volatile uint8_t buffer_count = 0;

// Exported map: Removed 'static' so hid.c can reference it if needed
const char scancode_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

/**
 * Shared function used by both PS/2 (callback) and USB (hid.c)
 * This fixes the "undefined reference to keyboard_buffer_add" error.
 */
void keyboard_buffer_add(char c) {
    if (!c) return;

    uint8_t next_head = (buffer_head + 1) % KEY_BUFFER_SIZE;

    // If buffer is full, we must drop the key
    if (next_head == buffer_tail) {
        return; 
    }

    key_buffer[buffer_head] = c;
    buffer_head = next_head;
    buffer_count++;

    // Echo to terminal
    terminal_putchar(c);
}

/**
 * Hardware Interrupt Handler for PS/2
 */
void keyboard_callback(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Ignore break codes (key release)
    if (scancode & 0x80) return; 

    char c = scancode_map[scancode];
    keyboard_buffer_add(c);
}

void keyboard_init(void) {
    buffer_head = 0;
    buffer_tail = 0;
    buffer_count = 0;
    // IRQ1 is usually mapped to IDT entry 33
    register_interrupt_handler(33, keyboard_callback); 
}

/**
 * Blocking read with a safe HLT to prevent CPU 100% usage
 */
char keyboard_get_char(void) {
    char c;

    while (1) {
        __asm__ __volatile__("cli"); // Disable interrupts to check buffer_count safely
        
        if (buffer_count > 0) {
            c = key_buffer[buffer_tail];
            buffer_tail = (buffer_tail + 1) % KEY_BUFFER_SIZE;
            buffer_count--;
            __asm__ __volatile__("sti"); // Re-enable interrupts after reading
            return c;
        }

        /* * IMPORTANT: 'sti' followed by 'hlt' is guaranteed by x86 
         * to execute 'hlt' BEFORE the next interrupt can trigger.
         * This prevents the race condition hang.
         */
        __asm__ __volatile__("sti; hlt"); 
    }
}
