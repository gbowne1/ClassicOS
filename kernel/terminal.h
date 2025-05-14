#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char *str);
void terminal_setcolor(uint8_t color);
void terminal_clear(void);
void update_cursor(void);

#endif
