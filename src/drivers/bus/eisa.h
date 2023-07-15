#ifndef EISA_H
#define EISA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the EISA bus
void eisa_init();

// Detect and configure EISA devices
void eisa_detect_devices();

// Read from an EISA device
uint8_t eisa_read(uint16_t port);

// Write to an EISA device
void eisa_write(uint16_t port, uint8_t value);

#endif