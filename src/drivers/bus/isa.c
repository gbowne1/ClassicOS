#include "isa.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h> // Include for va_list and related functions
#include <stdio.h>
#include <string.h>

// ISA bus controller base address
#define ISA_BASE_ADDRESS 0x0000

// ISA bus controller data port
#define ISA_DATA_PORT 0x00

// ISA bus controller command port
#define ISA_COMMAND_PORT 0x01

// Initialize the ISA bus
void isa_init()
{
    // Add any necessary initialization code here
}

// Detect and configure ISA devices
void isa_detect_devices()
{
    // Add any necessary device detection and configuration code here
}

// Read from an ISA device
uint8_t isa_read(uint16_t port)
{
    uint8_t value;

    // Check for invalid port
    if (port > 0xFF)
    {
        // Prepare a buffer to hold the formatted string
        char buffer[64];
        va_list args;
        va_start(args, port);
        vsnprintf(buffer, sizeof(buffer), "Invalid port address: 0x%x\n", port);
        va_end(args);

        // Use vfprintf to print the contents of the buffer to stderr
        vfprintf(stderr, buffer, NULL);
        return 0xFF; // Indicate error
    }

    // Read from the specified port with improved assembly attributes
    __asm__ volatile("inb %1, %0"
                     : "=r"(value)
                     : "Nd"(port)
                     : "memory");

    return value;
}

// Write to an ISA device
void isa_write(uint16_t port, uint8_t value)
{
    // Check for invalid port
    if (port > 0xFF)
    {
        // Prepare a buffer to hold the formatted string
        char buffer[64];
        va_list args;
        va_start(args, port);
        vsnprintf(buffer, sizeof(buffer), "Invalid port address: 0x%x\n", port);
        va_end(args);

        // Use vfprintf to print the contents of the buffer to stderr
        vfprintf(stderr, buffer, NULL);
        return;
    }

    // Write to the specified port with improved assembly attributes
    __asm__ volatile("outb %0, %1"
                     :
                     : "r"(value), "Nd"(port)
                     : "memory");
}
