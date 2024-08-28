#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t buttons;
} mouse_data_t;


void init_mouse(uint16_t com);
int mouse_received(uint16_t com);
mouse_data_t read_mouse(uint16_t com);

#endif
