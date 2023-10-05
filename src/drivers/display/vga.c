#include "vga.h"
#include <stddef.h>
#include <stdint.h>

#define VGA_MEMORY_ADDRESS 0xA0000
#define VGA_MEMORY_SIZE 0x60000

static uint8_t* vga_memory = (uint8_t*) VGA_MEMORY_ADDRESS;
static uint16_t vga_width = VGA_WIDTH;
static uint16_t vga_height = VGA_HEIGHT;
static uint8_t vga_depth = VGA_DEPTH;

void vga_init() {
    // Initialize VGA driver here
}

void vga_set_resolution(uint16_t width, uint16_t height, uint8_t depth) {
    // Set VGA resolution here
}

void vga_draw_pixel(uint16_t x, uint16_t y, uint8_t color) {
    // Draw a pixel on the screen at (x, y) with the given color
    uint32_t offset = y * vga_width + x;
    vga_memory[offset] = color;
}

void vga_clear_screen(uint8_t color) {
    // Clear the screen with the given color
    for (uint32_t i = 0; i < vga_width * vga_height; i++) {
        vga_memory[i] = color;
    }
}