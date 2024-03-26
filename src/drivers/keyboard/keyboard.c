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

// Function to translate the combined extended scancode (first byte + second byte)
uint8_t translate_extended_scancode(uint8_t second_scancode)
{
    uint16_t combined_scancode = (0xE0 << 8) | second_scancode;

    switch(combined_scancode)
    {
        case 0xE04D:
            return KEYCODE_PRINT_SCREEN;
        // Add more cases for other extended scancodes here
        default:
            return KEYCODE_UNKNOWN;
	}
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
		[0x0F] = KEYCODE_TAB,
		[0x10] = KEYCODE_Q,
		[0x11] = KEYCODE_W,
		[0x12] = KEYCODE_E,
		[0x13] = KEYCODE_R,
		[0x14] = KEYCODE_T,
		[0x15] = KEYCODE_Y,
		[0x16] = KEYCODE_U,
		[0x17] = KEYCODE_I,
		[0x18] = KEYCODE_O,
		[0x19] = KEYCODE_P,
		[0x1A] = KEYCODE_LEFT_BRACKET,
		[0x1B] = KEYCODE_RIGHT_BRACKET,
		[0x1C] = KEYCODE_ENTER,
		[0x1D] = KEYCODE_LEFT_CTRL,
		[0x1E] = KEYCODE_A,
		[0x1F] = KEYCODE_S,
		[0x20] = KEYCODE_D,
		[0x21] = KEYCODE_F,
		[0x22] = KEYCODE_G,
		[0x23] = KEYCODE_H,
		[0x24] = KEYCODE_J,
		[0x25] = KEYCODE_K,
		[0x26] = KEYCODE_L,
		[0x27] = KEYCODE_SEMICOLON,
		[0x28] = KEYCODE_APOSTROPHE,
		[0x29] = KEYCODE_GRAVE_ACCENT,
		[0x2A] = KEYCODE_LEFT_SHIFT,
		[0x2B] = KEYCODE_BACKSLASH,
		[0x2C] = KEYCODE_Z,
		[0x2D] = KEYCODE_X,
		[0x2E] = KEYCODE_C,
		[0x2F] = KEYCODE_V,
		[0x30] = KEYCODE_B,
		[0x31] = KEYCODE_N,
		[0x32] = KEYCODE_M,
		[0x33] = KEYCODE_COMMA,
		[0x34] = KEYCODE_DOT,
		[0x35] = KEYCODE_FORWARD_SLASH,
		[0x36] = KEYCODE_RIGHT_SHIFT,
		[0x37] = KEYCODE_PRINT_SCREEN, // Example extended scancode (replace with more)
		[0x38] = KEYCODE_LEFT_ALT,
		[0x39] = KEYCODE_SPACE,
		[0x3A] = KEYCODE_CAPS_LOCK,
		[0x3B] = KEYCODE_F1,
		[0x3C] = KEYCODE_F2,
		[0x3D] = KEYCODE_F3,
		[0x3E] = KEYCODE_F4,
		[0x3F] = KEYCODE_F5,
		[0x40] = KEYCODE_F6,
		[0x41] = KEYCODE_F7,
		[0x41] = KEYCODE_F7,
		[0x42] = KEYCODE_F8,
		[0x43] = KEYCODE_F9,
		[0x44] = KEYCODE_F10,
		[0x45] = KEYCODE_NUM_LOCK,	  // Extended scancode
		[0x46] = KEYCODE_SCROLL_LOCK, // Extended scancode
		[0x47] = KEYCODE_HOME,
		[0x48] = KEYCODE_UP,
		[0x49] = KEYCODE_PAGE_UP,
		[0x4A] = KEYCODE_MINUS_PAD,
		[0x4B] = KEYCODE_LEFT_ARROW,
		[0x4C] = KEYCODE_5_PAD,
		[0x4D] = KEYCODE_RIGHT_ARROW,
		[0x4E] = KEYCODE_PLUS_PAD,
		[0x4F] = KEYCODE_END,
		[0x50] = KEYCODE_DOWN_ARROW,
		[0x51] = KEYCODE_PAGE_DOWN,
		[0x52] = KEYCODE_INSERT,
		[0x53] = KEYCODE_DELETE,

		// ... (complete the rest based on the scancode table)
		[0xE0] = 0, // Handle extended scancodes (e.g., Print Screen) separately
	};

	static bool extended_scancode = false;
	static uint8_t second_scancode;

	if (extended_scancode)
	{
		// Handle second byte of extended scancode
		extended_scancode = false;
		return translate_extended_scancode(scancode); // Implement separate function for extended keycode translation
	}
	else if (scancode == 0xE0)
	{
		// First byte of extended scancode sequence
		extended_scancode = true;
		return 0; // Indicate incomplete scancode (waiting for second byte)
	}
	else
	{
		// Regular scancode
		if (scancode < sizeof(keycode_map))
		{
			// Map scancode directly to keycode
			return keycode_map[scancode];
		}
		else
		{
			// Handle unknown scancode
			return KEYCODE_UNKNOWN;
		}
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
