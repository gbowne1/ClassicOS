#ifndef MCA_H
#define MCA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the MCA bus
void mca_init();

// Detect and configure MCA devices
void mca_detect_devices();

// Read from an MCA device
uint8_t mca_read(uint16_t port);

// Write to an MCA device
void mca_write(uint16_t port, uint8_t value);

#endif