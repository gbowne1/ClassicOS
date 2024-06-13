#include "eisa.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MY_DEVICE_VENDOR_ID  0x1234
#define MY_DEVICE_DEVICE_ID  0x5678
#define MY_DEVICE_CLASS_CODE 0x90AB

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

						//printf("Config1: %u\n", config1);
                        //printf("Config2: %u\n", config2);
                        // Do something with the configuration data

						// Check for specific bits in config1
						if (config1 & 0x00000001) {
						// Enable feature 1 based on bit 0 of config1
						eisa_write(0xspecific_port_1, 0xvalue_to_enable_feature_1);
						}

						if (config1 & 0x00000010) {
						// Set DMA channel based on bits 4-5 of config1
						uint8_t dma_channel = (config1 >> 4) & 0x03;
						eisa_write(0xspecific_port_2, dma_channel);
						}

						// Check for specific bits in config2
						if (config2 & 0x00000001) {
						// Configure interrupt line based on bit 0 of config2
						eisa_write(0xspecific_port_3, 0xinterrupt_line_number);
						}
                    }
                }
            }
        }
    }
}

// Read a double word (32 bits) from an EISA device's configuration space
uint32_t eisa_read_config_dword(uint32_t address, uint8_t offset)
{
    // Set the EISA bus controller base address
    eisa_write(EISA_BASE_ADDRESS, address);

    // Set the EISA bus controller command port to read configuration data
    eisa_write(EISA_COMMAND_PORT, 0x80 | (offset & 0x03));

    // Read the double word from the EISA bus controller data port
    uint32_t value = 0;
    for (int i = 0; i < 4; i++)
    {
        value |= (eisa_read(EISA_DATA_PORT) << (i * 8));
    }

    return value;
}

// Read a word (16 bits) from an EISA device's configuration space
uint16_t eisa_read_config_word(uint32_t address, uint8_t offset)
{
    // Set the EISA bus controller base address
    eisa_write(EISA_BASE_ADDRESS, address);

    // Set the EISA bus controller command port to read configuration data
    eisa_write(EISA_COMMAND_PORT, 0x80 | (offset & 0x03));

    // Read the word from the EISA bus controller data port
    uint16_t value = 0;
    for (int i = 0; i < 2; i++)
    {
        value |= (eisa_read(EISA_DATA_PORT) << (i * 8));
    }

    return value;
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
