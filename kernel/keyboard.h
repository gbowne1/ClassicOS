#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stint.h>

void keyboard_init(void);
void keyboard_buffer_add(char c);
char keyboard_get_char(void);  // Blocking read from buffer
extern const char scancode_map[128];

#endif
