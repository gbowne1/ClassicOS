#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void *base;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
    bool initialized;
} framebuffer_t;

void framebuffer_init(framebuffer_t *fb, void *base, uint32_t width, uint32_t height, uint32_t pitch, uint8_t bpp);
uint32_t framebuffer_pack_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void framebuffer_put_pixel(framebuffer_t *fb, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void framebuffer_clear(framebuffer_t *fb, uint8_t r, uint8_t g, uint8_t b);
void framebuffer_fill_rect(framebuffer_t *fb, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
void framebuffer_blit(framebuffer_t *fb, uint32_t dst_x, uint32_t dst_y, const void *src, uint32_t src_w, uint32_t src_h, uint32_t src_pitch);
void framebuffer_test_pattern(framebuffer_t *fb);

#endif /* FRAMEBUFFER_H */
