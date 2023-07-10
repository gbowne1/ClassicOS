#include "screen.h"
#include <dos.h>

void screen_init() {
    // Initialize the screen driver
    // Add any necessary initialization code here
}

void set_40_column_mode() {
    // Set the screen mode to 40 columns
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    int86(0x10, &regs, &regs);
}

void set_80_column_mode() {
    // Set the screen mode to 80 columns
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    int86(0x10, &regs, &regs);

    regs.h.ah = 0x00;
    regs.h.al = 0x07;
    int86(0x10, &regs, &regs);
}