#ifndef ISA_H
#define ISA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the ISA bus
void isa_init();

// Detect and configure ISA devices
void isa_detect_devices();

// Read from an ISA device
uint8_t isa_read(uint16_t port);

// Write to an ISA device
void isa_write(uint16_t port, uint8_t value);

#endif