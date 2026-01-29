#ifndef CMOS_H
#define CMOS_H

#include <stdint.h>

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} cmos_time_t;

void cmos_read_time(cmos_time_t* time);
void cmos_print_time(cmos_time_t* time);

#endif
