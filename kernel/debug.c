#include "debug.h"
#include "vga.h"
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// VGA text mode color attributes
#define COLOR_WHITE 0x07

// Pointer to the VGA memory
volatile uint16_t* vga_buffer = (uint16_t*)VGA_MEMORY;

// Function to print a string to the VGA text buffer
void debug_print(const char *str) {
    while (*str) {
        if (*str == '\n') {
            // Handle new line
            // Move to the next line (not implemented here)
            // You can implement line wrapping if needed
            str++;
            continue;
        }

        // Calculate the position in the VGA buffer
        static int cursor_x = 0;
        static int cursor_y = 0;

        // Write the character and its attribute to the VGA buffer
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (COLOR_WHITE << 8) | *str;

        // Move the cursor to the right
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= VGA_HEIGHT) {
                cursor_y = 0; // Scroll up (not implemented here)
            }
        }

        str++;
    }
}
