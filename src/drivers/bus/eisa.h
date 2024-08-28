#ifndef EISA_H
#define EISA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

// Initialize the EISA bus
void eisa_init();

// Detect and configure EISA devices
void eisa_detect_devices();

// Read a double word (32 bits) from an EISA device's configuration space
uint32_t eisa_read_config_dword(uint32_t address, uint8_t offset);

// Read a word (16 bits) from an EISA device's configuration space
uint16_t eisa_read_config_word(uint32_t address, uint8_t offset);

// Read from an EISA device
uint8_t eisa_read(uint16_t port);

// Write to an EISA device
void eisa_write(uint16_t port, uint8_t value);

#endif
