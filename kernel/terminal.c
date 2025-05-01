#include <stdint.h>
#include "io.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static uint16_t* const vga_buffer = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t) color << 8 | (uint8_t) c;
}

void terminal_initialize(void) {
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', WHITE_ON_BLACK);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const size_t index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = vga_entry(c, WHITE_ON_BLACK);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    // Scroll if needed
    if (cursor_y >= VGA_HEIGHT) {
        for (uint16_t y = 1; y < VGA_HEIGHT; y++) {
            for (uint16_t x = 0; x < VGA_WIDTH; x++) {
                vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
            }
        }

        // Clear the last line
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', WHITE_ON_BLACK);
        }

        cursor_y = VGA_HEIGHT - 1;
    }
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}
