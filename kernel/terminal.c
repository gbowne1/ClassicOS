#include <stdint.h>
#include "io.h"
#include "terminal.h"
#include "vga.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static uint16_t* const vga_buffer = (uint16_t*) VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t current_color = WHITE_ON_BLACK;
static uint16_t last_cursor_pos = 0xFFFF;

void terminal_initialize(void) {
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', current_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(); // Optional: good idea to reset position
}

void terminal_putchar(char c) {
    // Handle backspace
    if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(' ', current_color);
        update_cursor(); // Optional, if you add cursor updating
        return;
    }

    // Handle newline
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        const size_t index = cursor_y * VGA_WIDTH + cursor_x;
        vga_buffer[index] = vga_entry(c, current_color);
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
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', current_color);
        }

        cursor_y = VGA_HEIGHT - 1;
    }

    update_cursor(); // Optional, if you want the hardware cursor to follow
}


void terminal_write(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        terminal_putchar(str[i]);
    }
}

void terminal_setcolor(uint8_t color)
{
    current_color = color;
}

void terminal_clear(void) {
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', current_color);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void update_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    if (pos == last_cursor_pos) return;
    last_cursor_pos = pos;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
