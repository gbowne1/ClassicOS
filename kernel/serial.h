#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void serial_init(void);
void serial_init_port(uint16_t port);
void serial_write_char(char c);
void serial_write(const char *str);
void serial_write_string(const char *str);

#endif