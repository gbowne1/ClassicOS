#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void init_serial(uint16_t com);
int serial_received(uint16_t com);
char read_serial(uint16_t com);
int is_transmit_empty(uint16_t com);
void write_serial(uint16_t com, char a);

#endif
