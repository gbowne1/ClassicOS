#include "screen.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Video memory address
#define VIDEO_MEMORY_ADDRESS 0xB8000

// Screen attributes (color)
static uint8_t screen_color =
    (SCREEN_COLOR_LIGHT_GRAY << 4) | SCREEN_COLOR_BLACK;

// Helper function to calculate the screen buffer offset
static inline uint16_t screen_offset(uint8_t x, uint8_t y)
{
    return (y * SCREEN_WIDTH + x) * 2;
}

// Initialize the screen driver
void screen_init()
{
    // No initialization required for this simple driver
}

// Clear the screen
void screen_clear()
{
    uint16_t *video_memory = (uint16_t *)VIDEO_MEMORY_ADDRESS;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        video_memory[i] = ' ' | (screen_color << 8);
    }
}

// Set the foreground and background colors
void screen_set_color(uint8_t foreground, uint8_t background)
{
    screen_color = (background << 4) | foreground;
}

// Print a character at the specified position
void screen_put_char(char c, uint8_t x, uint8_t y)
{
    uint16_t *video_memory = (uint16_t *)VIDEO_MEMORY_ADDRESS;
    uint16_t  offset       = screen_offset(x, y);
    video_memory[offset]   = c | (screen_color << 8);
}

// Print a null-terminated string at the specified position
void screen_put_string(const char *str, uint8_t x, uint8_t y)
{
    uint16_t *video_memory = (uint16_t *)VIDEO_MEMORY_ADDRESS;
    uint16_t  offset       = screen_offset(x, y);
    while (*str)
    {
        video_memory[offset] = *str | (screen_color << 8);
        str++;
        offset += 2;
    }
}