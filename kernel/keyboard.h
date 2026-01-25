#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void keyboard_init(void);
void keyboard_buffer_add(char c);
char keyboard_get_char(void);  

extern const char scancode_map[128]; 

#endif
