#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void serial_init(void);
void serial_write(char c);
void serial_write_string(const char *str);

#endif
