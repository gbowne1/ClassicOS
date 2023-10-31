#include "keyboard.h"
#include "../io/io.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Keyboard input buffer
#define KEYBOARD_BUFFER_SIZE          32
#define KEYBOARD_DATA_PORT            0x60
#define KEYBOARD_INTERRUPT_VECTOR     0x09
#define KEYBOARD_COMMAND_PORT         0x64
#define KEYBOARD_DATA_PORT            0x60
#define KEYBOARD_ENABLE_COMMAND       0xAE
#define KEYBOARD_ENABLE_SCANCODE      0xF4
#define KEYBOARD_ACKNOWLEDGE_SCANCODE 0xFA

static uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t  keyboard_buffer_head = 0;
static size_t  keyboard_buffer_tail = 0;

void set_interrupt_vector(uint8_t vector, void (*handler)());
void enable_interrupt(uint8_t vector);

void KeyboardInterruptHandler()
{
	uint8_t scancode = inb(KEYBOARD_DATA_PORT);

	// Add scancode to buffer
	keyboard_buffer[keyboard_buffer_head] = scancode;
	keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

// Initialize keyboard
void keyboard_init()
{
    // Install keyboard interrupt handler
    //set_interrupt_vector(KEYBOARD_INTERRUPT_VECTOR, KeyboardInterruptHandler);
    //enable_interrupt(KEYBOARD_INTERRUPT_VECTOR);

    // Enable keyboard
    outb(KEYBOARD_COMMAND_PORT, KEYBOARD_ENABLE_COMMAND);
    while (inb(KEYBOARD_COMMAND_PORT) & 0x02)
        ;
    outb(KEYBOARD_DATA_PORT, KEYBOARD_ENABLE_SCANCODE);
    while (inb(KEYBOARD_DATA_PORT) != KEYBOARD_ACKNOWLEDGE_SCANCODE)
        ;
}

// Check if keyboard buffer is empty
bool keyboard_buffer_empty()
{
    return keyboard_buffer_head == keyboard_buffer_tail;
}

// Read scancode from keyboard buffer
uint8_t keyboard_read_scancode()
{
    if (keyboard_buffer_empty())
    {
        return 0;
    }

    uint8_t scancode     = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return scancode;
}