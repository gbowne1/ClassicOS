#include "keyboard.h"
#include "../io/io.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Keyboard input buffer
#define KEYBOARD_BUFFER_SIZE 32
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_INTERRUPT_VECTOR 0x09
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_ENABLE_COMMAND 0xAE
#define KEYBOARD_ENABLE_SCANCODE 0xF4
#define KEYBOARD_ACKNOWLEDGE_SCANCODE 0xFA
#define KEYCODE_UNKNOWN 0xFF
#define KEYCODE_C 0x03

static uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_head = 0;
static size_t keyboard_buffer_tail = 0;

void set_interrupt_vector(uint8_t vector, void (*handler)());
void enable_interrupt(uint8_t vector);

void KeyboardInterruptHandler()
{
	uint8_t scancode = inb(KEYBOARD_DATA_PORT);
	uint8_t keycode = translate_scancode_to_keycode(scancode);

	// Add scancode to buffer
	keyboard_buffer[keyboard_buffer_head] = scancode;
	keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

uint8_t translate_scancode_to_keycode(uint8_t scancode)
{
	static uint8_t keycode_map[128] = {
		[0x01] = KEYCODE_ESC,
		[0x02] = KEYCODE_1,
		[0x03] = KEYCODE_2,
		[0x04] = KEYCODE_3,
		[0x05] = KEYCODE_4,
		[0x06] = KEYCODE_5,
		[0x07] = KEYCODE_6,
		[0x08] = KEYCODE_7,
		[0x09] = KEYCODE_8,
		[0x0A] = KEYCODE_9,
		[0x0B] = KEYCODE_0,
		[0x0C] = KEYCODE_MINUS,
		[0x0D] = KEYCODE_EQUALS,
		[0x0E] = KEYCODE_BACKSPACE,
		// ... (complete the rest based on the scancode table)
		[0xE0] = 0, // Handle extended scancodes (e.g., Print Screen) separately
	};

	if (scancode < sizeof(keycode_map))
	{
		// Map scancode directly to keycode
		return keycode_map[scancode];
	}
	else
	{
		// Handle unknown scancode
		return KEYCODE_UNKNOWN; // Or return a special keycode indicating error
	}
}

// Initialize keyboard
void keyboard_init()
{
	// Install keyboard interrupt handler
	set_interrupt_vector(KEYBOARD_INTERRUPT_VECTOR, KeyboardInterruptHandler);
	enable_interrupt(KEYBOARD_INTERRUPT_VECTOR);

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

	uint8_t scancode = keyboard_buffer[keyboard_buffer_tail];
	keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
	return scancode;
}
