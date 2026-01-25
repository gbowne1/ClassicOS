#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
char keyboard_get_char(void);  // Blocking read from buffer
extern const char scancode_map[128];

#endif
