#ifndef VESA_H
#define VESA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the VESA bus
void vesa_init();

// Detect and configure VESA devices
void vesa_detect_devices();

// Read from a VESA device
uint8_t vesa_read(uint16_t port);

// Write to a VESA device
void vesa_write(uint16_t port, uint8_t value);

#endif