#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define VGA_WIDTH 640
#define VGA_HEIGHT 480
#define VGA_DEPTH 8

void vga_init();
void vga_set_resolution(uint16_t width, uint16_t height, uint8_t depth);
void vga_draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void vga_clear_screen(uint8_t color);

#endif