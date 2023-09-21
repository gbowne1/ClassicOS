#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

void    keyboard_init();
bool    keyboard_buffer_empty();
uint8_t keyboard_read_scancode();
void    set_interrupt_vector(uint8_t vector, void (*handler)());
void    enable_interrupt(uint8_t vector);

#endif