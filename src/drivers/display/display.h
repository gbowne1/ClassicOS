#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the display driver
void display_init();

// Set the display mode
void display_set_mode(uint32_t width, uint32_t height, uint32_t bpp);

// Draw a pixel at the specified coordinates
void display_draw_pixel(uint32_t x, uint32_t y, uint32_t color);

// Clear the display with the specified color
void display_clear(uint32_t color);

// Draw a character at the specified coordinates
void display_draw_char(uint32_t x, uint32_t y, char c, uint32_t color);

// Draw a string at the specified coordinates
void display_draw_string(uint32_t x, uint32_t y, const char *str,
                         uint32_t color);

#endif