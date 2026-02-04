#include "cmos.h"
#include "io.h"
#include "print.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

enum {
    CMOS_SEC  = 0x00,
    CMOS_MIN  = 0x02,
    CMOS_HOUR = 0x04,
    CMOS_DAY  = 0x07,
    CMOS_MONTH= 0x08,
    CMOS_YEAR = 0x09,
    CMOS_STAT_A = 0x0A,
    CMOS_STAT_B = 0x0B
};

// Check if CMOS is currently updating its values
static int is_updating() {
    outb(CMOS_ADDR, CMOS_STAT_A);
    return (inb(CMOS_DATA) & 0x80);
}

static uint8_t get_register(int reg) {
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

void cmos_read_time(cmos_time_t* time) {
    // Wait for any current update to finish
    while (is_updating());

    uint8_t sec   = get_register(CMOS_SEC);
    uint8_t min   = get_register(CMOS_MIN);
    uint8_t hour  = get_register(CMOS_HOUR);
    uint8_t day   = get_register(CMOS_DAY);
    uint8_t month = get_register(CMOS_MONTH);
    uint8_t year  = get_register(CMOS_YEAR);
    uint8_t statb = get_register(CMOS_STAT_B);

    // If Bit 2 of Status Register B is 0, then values are BCD
    if (!(statb & 0x04)) {
        time->second = (sec & 0x0F) + ((sec / 16) * 10);
        time->minute = (min & 0x0F) + ((min / 16) * 10);
        time->hour   = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
        time->day    = (day & 0x0F) + ((day / 16) * 10);
        time->month  = (month & 0x0F) + ((month / 16) * 10);
        time->year   = (year & 0x0F) + ((year / 16) * 10);
    } else {
        time->second = sec;
        time->minute = min;
        time->hour   = hour;
        time->day    = day;
        time->month  = month;
        time->year   = year;
    }

    // Adjust for Century (assuming we are in the 2000s for ClassicOS)
    time->year += 2000; 
}

void cmos_print_time(cmos_time_t* time) {
    // Using your print_string/itoa style logic
    char buf[16];
    
    print_string("System Time: ");
    
    // Simple padding check for minutes
    print_hex(time->hour, 0, 1);
    print_string(":");
    if (time->minute < 10) print_string("0");
    print_hex(time->minute, 0, 1);
    print_string(":");
    if (time->second < 10) print_string("0");
    print_hex(time->second, 0, 1);
    
    print_string("  ");
    
    print_hex(time->month, 0, 1);
    print_string("/");
    print_hex(time->day, 0, 1);
    print_string("/");
    print_hex(time->year, 0, 1);
    print_string("\n");
}
