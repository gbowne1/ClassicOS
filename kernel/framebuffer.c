#include "framebuffer.h"
#include <stddef.h>
#include <stdint.h>

// Simple init
void framebuffer_init(framebuffer_t *fb, void *base, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp) {
    fb->base = base;
    fb->width = width;
    fb->height = height;
    fb->pitch = pitch;
    fb->bpp = bpp;
    fb->initialized = true;
}

// Pack color into 32-bit value. Format: 0xAARRGGBB
uint32_t framebuffer_pack_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

void framebuffer_put_pixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!fb->initialized) return;
    if (x >= fb->width || y >= fb->height) return;
    if (fb->bpp != 32) return; // only 32bpp implemented here

    uint8_t *line = (uint8_t*)fb->base + (size_t)y * fb->pitch;
    uint32_t *pixel = (uint32_t*)(line + x * 4);
    *pixel = framebuffer_pack_color(r, g, b, a);
}

void framebuffer_clear(framebuffer_t *fb, uint8_t r, uint8_t g, uint8_t b) {
    if (!fb->initialized) return;
    if (fb->bpp != 32) return;

    uint32_t color = framebuffer_pack_color(r,g,b,0xFF);
    for (uint32_t y = 0; y < fb->height; ++y) {
        uint32_t *row = (uint32_t*)((uint8_t*)fb->base + (size_t)y * fb->pitch);
        for (uint32_t x = 0; x < fb->width; ++x) {
            row[x] = color;
        }
    }
}

void framebuffer_fill_rect(framebuffer_t *fb, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b) {
    if (!fb->initialized) return;
    if (fb->bpp != 32) return;

    if (x >= fb->width || y >= fb->height) return;
    if (x + w > fb->width) w = fb->width - x;
    if (y + h > fb->height) h = fb->height - y;

    uint32_t color = framebuffer_pack_color(r,g,b,0xFF);
    for (uint32_t yy = 0; yy < h; ++yy) {
        uint32_t *row = (uint32_t*)((uint8_t*)fb->base + (size_t)(y + yy) * fb->pitch) + x;
        for (uint32_t xx = 0; xx < w; ++xx) {
            row[xx] = color;
        }
    }
}

// Simple blit from a source buffer with 32bpp pixels and given pitch (bytes per line)
void framebuffer_blit(framebuffer_t *fb, uint32_t dst_x, uint32_t dst_y, const void *src, uint32_t src_w, uint32_t src_h, uint32_t src_pitch) {
    if (!fb->initialized) return;
    if (fb->bpp != 32) return;

    if (dst_x >= fb->width || dst_y >= fb->height) return;

    uint32_t copy_w = src_w;
    uint32_t copy_h = src_h;
    if (dst_x + copy_w > fb->width) copy_w = fb->width - dst_x;
    if (dst_y + copy_h > fb->height) copy_h = fb->height - dst_y;

    const uint8_t *s = (const uint8_t*)src;
    for (uint32_t yy = 0; yy < copy_h; ++yy) {
        uint32_t *dst_row = (uint32_t*)((uint8_t*)fb->base + (size_t)(dst_y + yy) * fb->pitch) + dst_x;
        const uint32_t *src_row = (const uint32_t*)(s + (size_t)yy * src_pitch);
        for (uint32_t xx = 0; xx < copy_w; ++xx) {
            dst_row[xx] = src_row[xx];
        }
    }
}

void framebuffer_test_pattern(framebuffer_t *fb) {
    if (!fb->initialized) return;
    // simple color bars
    uint32_t band_h = fb->height / 6;
    framebuffer_fill_rect(fb, 0, 0, fb->width, band_h, 0xFF, 0x00, 0x00); // red
    framebuffer_fill_rect(fb, 0, band_h, fb->width, band_h, 0x00, 0xFF, 0x00); // green
    framebuffer_fill_rect(fb, 0, band_h*2, fb->width, band_h, 0x00, 0x00, 0xFF); // blue
    framebuffer_fill_rect(fb, 0, band_h*3, fb->width, band_h, 0xFF, 0xFF, 0x00); // yellow
    framebuffer_fill_rect(fb, 0, band_h*4, fb->width, band_h, 0xFF, 0x00, 0xFF); // magenta
    framebuffer_fill_rect(fb, 0, band_h*5, fb->width, fb->height - band_h*5, 0x00, 0xFF, 0xFF); // cyan
}
