#include "keyboard.h"
#include "../io/io.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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
#define KEYCODE_ESC 0x01
#define KEYCODE_1 0x02
#define KEYCODE_2 0x03
#define KEYCODE_3 0x04
#define KEYCODE_4 0x05
#define KEYCODE_5 0x06
#define KEYCODE_6 0x07
#define KEYCODE_7 0x08
#define KEYCODE_8 0x09
#define KEYCODE_9 0x0A
#define KEYCODE_0 0x0B
#define KEYCODE_MINUS 0x0C
#define KEYCODE_EQUALS 0x0D
#define KEYCODE_BACKSPACE 0x0E
#define KEYCODE_TAB 0x0F
#define KEYCODE_Q 0x10
#define KEYCODE_W 0x11
#define KEYCODE_E 0x12
#define KEYCODE_R 0x13
#define KEYCODE_T 0x14
#define KEYCODE_Y 0x15
#define KEYCODE_U 0x16
#define KEYCODE_I 0x17
#define KEYCODE_O 0x18
#define KEYCODE_P 0x19
#define KEYCODE_LEFT_BRACKET 0x1A
#define KEYCODE_RIGHT_BRACKET 0x1B
#define KEYCODE_ENTER 0x1C
#define KEYCODE_LEFT_CTRL 0x1D
#define KEYCODE_A 0x1E
#define KEYCODE_S 0x1F
#define KEYCODE_D 0x20
#define KEYCODE_F 0x21
#define KEYCODE_G 0x22
#define KEYCODE_H 0x23
#define KEYCODE_J 0x24
#define KEYCODE_K 0x25
#define KEYCODE_L 0x26
#define KEYCODE_SEMICOLON 0x27
#define KEYCODE_APOSTROPHE 0x28
#define KEYCODE_GRAVE_ACCENT 0x29
#define KEYCODE_LEFT_SHIFT 0x2A
#define KEYCODE_BACKSLASH 0x2B
#define KEYCODE_Z 0x2C
#define KEYCODE_X 0x2D
#define KEYCODE_V 0x2F
#define KEYCODE_B 0x30
#define KEYCODE_N 0x31
#define KEYCODE_M 0x32
#define KEYCODE_COMMA 0x33
#define KEYCODE_DOT 0x34
#define KEYCODE_FORWARD_SLASH 0x35
#define KEYCODE_RIGHT_SHIFT 0x36
#define KEYCODE_PRINT_SCREEN 0x37
#define KEYCODE_LEFT_ALT 0x38
#define KEYCODE_SPACE 0x39
#define KEYCODE_CAPS_LOCK 0x3A
#define KEYCODE_F1 0x3B
#define KEYCODE_F2 0x3C
#define KEYCODE_F3 0x3D
#define KEYCODE_F4 0x3E
#define KEYCODE_F5 0x3F
#define KEYCODE_F6 0x40
#define KEYCODE_F7 0x41
#define KEYCODE_F8 0x42
#define KEYCODE_F9 0x43
#define KEYCODE_F10 0x44
#define KEYCODE_NUM_LOCK 0x45
#define KEYCODE_SCROLL_LOCK 0x46
#define KEYCODE_HOME 0x47
#define KEYCODE_UP 0x48
#define KEYCODE_PAGE_UP 0x49
#define KEYCODE_MINUS_PAD 0x4A
#define KEYCODE_LEFT_ARROW 0x4B
#define KEYCODE_5_PAD 0x4C
#define KEYCODE_RIGHT_ARROW 0x4D
#define KEYCODE_PLUS_PAD 0x4E
#define KEYCODE_END 0x4F
#define KEYCODE_DOWN_ARROW 0x50
#define KEYCODE_PAGE_DOWN 0x51
#define KEYCODE_INSERT 0x52
#define KEYCODE_DELETE 0x53
#define KEYCODE_LEFT_CTRL_BREAK 0x57
#define KEYCODE_PAUSE 0x5F
#define KEYCODE_RIGHT 0x62
#define KEYCODE_DOWN 0x64
#define KEYCODE_F11 0x66
#define KEYCODE_F12 0x67
#define KEYCODE_LED_NUM_LOCK 0x69
#define KEYCODE_LED_CAPS_LOCK 0x6A
#define KEYCODE_LED_SCROLL_LOCK 0x6B
#define KEYCODE_EXTENDED 0xE0

static uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_head = 0;
static size_t keyboard_buffer_tail = 0;

void set_interrupt_vector(uint8_t vector, void (*handler)());
void enable_interrupt(uint8_t vector);
void keyboard_init();
bool keyboard_buffer_empty();
uint8_t keyboard_read_scancode();
uint8_t translate_scancode_to_keycode(uint8_t scancode);

bool keyboard_buffer_full()
{
    return (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE == keyboard_buffer_tail;
}

void KeyboardInterruptHandler()
{
    if (!keyboard_buffer_full())
    {
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);
        uint8_t keycode = translate_scancode_to_keycode(scancode);

        // Use the keycode for further processing
        process_keycode(keycode);

        // Or log the keycode for debugging
        log_keycode(keycode);

        // Add scancode to buffer
        keyboard_buffer[keyboard_buffer_head] = scancode;
        keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    }
}

void process_keycode(uint8_t keycode)
{
    // Add your keycode processing logic here
    // For example, you could update the system state based on specific key presses
    switch(keycode)
    {
        case KEYCODE_ESC:
            // Handle ESC key press
            break;
        case KEYCODE_ENTER:
            // Handle ENTER key press
            break;
        // Add more cases as needed
    }
}

// Example function to log the keycode
void log_keycode(uint8_t keycode)
{
    // Add your logging logic here
    // For example, you could print the keycode to a debug console or write it to a log file
    printf("Keycode pressed: 0x%02X\n", keycode);
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
	static uint8_t keycode_map[256] = {
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
		[0x54] = KEYCODE_HOME,
		[0x55] = KEYCODE_UP,
		[0x56] = KEYCODE_PAGE_UP,
		[0x57] = KEYCODE_LEFT_CTRL_BREAK,  // Handle Break key (adjust if needed)
		[0x58] = KEYCODE_RIGHT_SHIFT,
		[0x59] = KEYCODE_NUM_LOCK,
		[0x5A] = KEYCODE_SCROLL_LOCK,
		[0x5B] = KEYCODE_F7,
		[0x5C] = KEYCODE_F8,
		[0x5D] = KEYCODE_F9,
		[0x5E] = KEYCODE_F10,
		[0x5F] = KEYCODE_PAUSE,  // Handle Pause key (adjust if needed)
		[0x60] = KEYCODE_INSERT,
		[0x61] = KEYCODE_DELETE,
		[0x62] = KEYCODE_RIGHT,
		[0x63] = KEYCODE_END,
		[0x64] = KEYCODE_DOWN,
		[0x65] = KEYCODE_PAGE_DOWN,
		[0x66] = KEYCODE_F11,
		[0x67] = KEYCODE_F12,
		[0x68] = KEYCODE_UNKNOWN,  // (unused)
		[0x69] = KEYCODE_LED_NUM_LOCK,  // Num Lock LED status
		[0x6A] = KEYCODE_LED_CAPS_LOCK, // Caps Lock LED status
		[0x6B] = KEYCODE_LED_SCROLL_LOCK,
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
