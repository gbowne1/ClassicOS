#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

void keyboard_init();
bool keyboard_buffer_empty();
uint8_t keyboard_read_scancode();

#endif