#include "vga.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>   // Include for vsnprintf
#include <string.h>  // Include for strlen
#include <stdarg.h>  // Include for va_list, va_start, etc.

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from the specified port
uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

uint8_t vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void vga_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * 80 + x;
    ((uint16_t*) 0xB8000)[index] = vga_entry(c, color);
}

void vga_clear(uint8_t color) {
    uint16_t blank = vga_entry(' ', color);
    for (size_t i = 0; i < 80 * 25; ++i) {
        ((uint16_t*) 0xB8000)[i] = blank;
    }
}

void vga_write_string(const char* data, size_t size) {
    size_t x = 0;
    size_t y = 0;
    for (size_t i = 0; i < size; ++i) {
        if (x >= 80) {  // If we reach the end of a line, move to the next line
            x = 0;
            y++;
        }
        
        if (y >= 25) {  // If we reach the bottom of the screen, scroll
            vga_scroll();
            y = 24;  // Reset to the last row
        }
        
        vga_put_entry_at(data[i], vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), x, y);
        x++;  // Move to the next column
    }
}

void vga_scroll(void) {
    uint16_t* video_memory = (uint16_t*)0xB8000;
    // Shift all lines up by one row (80 columns per row)
    for (size_t i = 0; i < 24 * 80; ++i) {
        video_memory[i] = video_memory[i + 80];  // Move one row up
    }

    // Clear the last row (bottom row)
    for (size_t i = 24 * 80; i < 25 * 80; ++i) {
        video_memory[i] = vga_entry(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE));
    }
}

void vga_set_cursor_position(size_t x, size_t y) {
    uint16_t position = y * 80 + x;  // Calculate linear index (y * 80 + x)

    // Set the high byte of the cursor position
    outb(VGA_PORT_INDEX, 0x0E);        // Cursor high byte register
    outb(VGA_PORT_DATA, position >> 8);

    // Set the low byte of the cursor position
    outb(VGA_PORT_INDEX, 0x0F);        // Cursor low byte register
    outb(VGA_PORT_DATA, position & 0xFF);
}

size_t vga_get_cursor_position(void) {
    outb(VGA_PORT_INDEX, 0x0E);         // Cursor high byte register
    uint8_t high = inb(VGA_PORT_DATA);  // Read high byte

    outb(VGA_PORT_INDEX, 0x0F);         // Cursor low byte register
    uint8_t low = inb(VGA_PORT_DATA);   // Read low byte

    return (high << 8) | low;  // Combine the high and low bytes
}

void vga_set_cursor_blinking(bool enable) {
    outb(VGA_PORT_INDEX, 0x0A);        // Cursor control register
    uint8_t cursor = inb(VGA_PORT_DATA);
    if (enable) {
        cursor |= 0x01;  // Enable blinking
    } else {
        cursor &= ~0x01; // Disable blinking
    }
    outb(VGA_PORT_INDEX, 0x0A);        // Cursor control register
    outb(VGA_PORT_DATA, cursor);
}

void vga_set_cursor_shape(uint8_t start, uint8_t end) {
    outb(VGA_PORT_INDEX, 0x0A);        // Cursor control register
    uint8_t cursor = inb(VGA_PORT_DATA);
    cursor = (cursor & 0xC0) | (start & 0x3F);  // Set start of cursor shape
    outb(VGA_PORT_DATA, cursor);

    outb(VGA_PORT_INDEX, 0x0B);        // Cursor start register
    outb(VGA_PORT_DATA, end & 0x3F);   // Set end of cursor shape
}

void vga_set_cursor_color(uint8_t color) {
    outb(VGA_PORT_INDEX, 0x0A);        // Cursor control register
    uint8_t cursor = inb(VGA_PORT_DATA);
    cursor = (cursor & 0xC0) | (color & 0x3F);  // Set cursor color
    outb(VGA_PORT_DATA, cursor);
}

void vga_set_cursor_blink_rate(uint8_t rate) {
    outb(VGA_PORT_INDEX, 0x0A);        // Cursor control register
    uint8_t cursor = inb(VGA_PORT_DATA);
    cursor = (cursor & 0xC0) | (rate & 0x3F);  // Set cursor blink rate
    outb(VGA_PORT_DATA, cursor);
}

void vga_printf(const char* format, ...) {
    char buffer[256];  // Buffer to store the formatted string
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    // Now you can use the buffer with vga_write_string
    vga_write_string(buffer, strlen(buffer));
}

void vga_init(void) {
    // Clear the screen
    vga_clear(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY));
    
    // Set the cursor to the top-left corner
    vga_set_cursor_position(0, 0);
    
    // Optionally, set cursor blinking and shape
    vga_set_cursor_blinking(true);
    vga_set_cursor_shape(0x20, 0x3F);  // Default shape
}

