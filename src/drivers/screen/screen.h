#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Screen dimensions
#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25

// Screen colors
#define SCREEN_COLOR_BLACK         0
#define SCREEN_COLOR_BLUE          1
#define SCREEN_COLOR_GREEN         2
#define SCREEN_COLOR_CYAN          3
#define SCREEN_COLOR_RED           4
#define SCREEN_COLOR_MAGENTA       5
#define SCREEN_COLOR_BROWN         6
#define SCREEN_COLOR_LIGHT_GRAY    7
#define SCREEN_COLOR_DARK_GRAY     8
#define SCREEN_COLOR_LIGHT_BLUE    9
#define SCREEN_COLOR_LIGHT_GREEN   10
#define SCREEN_COLOR_LIGHT_CYAN    11
#define SCREEN_COLOR_LIGHT_RED     12
#define SCREEN_COLOR_LIGHT_MAGENTA 13
#define SCREEN_COLOR_YELLOW        14
#define SCREEN_COLOR_WHITE         15

// Initialize the screen driver
void screen_init();

// Clear the screen
void screen_clear();

// Set the foreground and background colors
void screen_set_color(uint8_t foreground, uint8_t background);

// Print a character at the specified position
void screen_put_char(char c, uint8_t x, uint8_t y);

// Print a null-terminated string at the specified position
void screen_put_string(const char *str, uint8_t x, uint8_t y);

#endif