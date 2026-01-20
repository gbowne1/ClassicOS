#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "vga.h" // Include VGA functions

#define VGA_PORT 0x3C0  // Base port for VGA (Often used for general control, though 0x3D4/0x3D5 are used for cursor)

// Function prototypes
void init_display(void);
void enumerate_displays(void);
void set_display_mode(uint8_t mode); // In this context, modes are typically BIOS or VESA modes, which are complex.
                                      // We'll treat this as a placeholder/simple mode call.
void clear_display(void);

// New function to write a string using the VGA driver
void display_write_string(const char* str);

// New function to print a formatted string using the VGA driver
void display_printf(const char* format, ...);

#endif // DISPLAY_H
