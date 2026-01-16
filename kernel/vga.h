#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>  // For va_list, va_start, etc.

// VGA color definitions
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color;

// VGA port addresses
typedef enum {
    VGA_PORT_INDEX = 0x3D4,  // Index register for VGA
    VGA_PORT_DATA  = 0x3D5   // Data register for VGA
} vga_io_port_t;

// Function prototypes
uint8_t vga_entry_color(vga_color fg, vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);
void vga_init(void);

void vga_put_entry_at(char c, uint8_t color, size_t x, size_t y);
void vga_clear(uint8_t color);
void vga_write_string(const char* data, size_t size);
void vga_scroll(void);
void vga_set_cursor_position(size_t x, size_t y);
size_t vga_get_cursor_position(void);

void vga_set_cursor_blinking(bool enable);
void vga_set_cursor_shape(uint8_t start, uint8_t end);
void vga_set_cursor_color(uint8_t color);
void vga_set_cursor_blink_rate(uint8_t rate);

void vga_printf(const char* format, ...);

#endif
