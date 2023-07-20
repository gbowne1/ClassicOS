#include "eisa.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// EISA bus controller base address
#define EISA_BASE_ADDRESS 0x0000

// EISA bus controller data port
#define EISA_DATA_PORT 0x00

// EISA bus controller command port
#define EISA_COMMAND_PORT 0x01

// Initialize the EISA bus
void eisa_init()
{
    // Add any necessary initialization code here
}

// Detect and configure EISA devices
void eisa_detect_devices()
{
    uint32_t bus, slot, func;
    uint16_t vendor_id, device_id, class_code;

    for (bus = 0; bus < 256; bus++)
    {
        for (slot = 0; slot < 32; slot++)
        {
            for (func = 0; func < 8; func++)
            {
                uint32_t address = (bus << 16) | (slot << 11) | (func << 8);
                uint32_t id      = eisa_read_config_dword(address, 0);
                vendor_id        = id & 0xFFFF;
                device_id        = (id >> 16) & 0xFFFF;
                class_code       = eisa_read_config_word(address, 10);
                if (vendor_id != 0xFFFF)
                {
                    // Device detected, do something with it
                    if (vendor_id == MY_DEVICE_VENDOR_ID &&
                        device_id == MY_DEVICE_DEVICE_ID &&
                        class_code == MY_DEVICE_CLASS_CODE)
                    {
                        // This is my device, configure it
                        uint32_t config1 = eisa_read_config_dword(address, 4);
                        uint32_t config2 = eisa_read_config_dword(address, 8);
                        // Do something with the configuration data
                    }
                }
            }
        }
    }
}

// Read from an EISA device
uint8_t eisa_read(uint16_t port)
{
    uint8_t value;

    // Read from the specified port
    __asm__ volatile("inb %1, %0" : "=a"(value) : "dN"(port));

    return value;
}

// Write to an EISA device
void eisa_write(uint16_t port, uint8_t value)
{
    // Write the specified value to the specified port
    __asm__ volatile("outb %0, %1" : : "a"(value), "dN"(port));
}