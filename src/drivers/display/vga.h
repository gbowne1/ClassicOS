#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    // Add other relevant settings like color palette, etc.
} vga_display_settings_t;

void vga_init();
void vga_set_resolution(uint16_t width, uint16_t height, uint8_t depth);
void vga_draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void vga_clear_screen(uint8_t color);

uint8_t read_vga_register(uint16_t register_address);
int inb(uint16_t port);
void outb(uint16_t port, uint8_t data);

#endif
