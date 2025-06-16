#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define VGA_PORT 0x3C0  // Base port for VGA

// Function prototypes
void init_display(void);
void enumerate_displays(void);
void set_display_mode(uint8_t mode);
void clear_display(void);

#endif // DISPLAY_H
