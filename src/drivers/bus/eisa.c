#include "eisa.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MY_DEVICE_VENDOR_ID 0x1234
#define MY_DEVICE_DEVICE_ID 0x5678
#define MY_DEVICE_CLASS_CODE 0x90AB

// EISA bus controller base address
#define EISA_BASE_ADDRESS 0x0000

// EISA bus controller data port
#define EISA_DATA_PORT 0x00

// EISA bus controller command port
#define EISA_COMMAND_PORT 0x01

// EISA bus controller interrupt control port
#define EISA_INTERRUPT_CONTROL_PORT 0x03 // Placeholder, replace with actual address

// EISA bus controller DMA channel configuration port
#define EISA_DMA_CHANNEL_CONFIG_PORT 0x04 // Placeholder, replace with actual address

// Initialize the EISA bus
void eisa_init()
{
	// Reset the EISA bus controller (if supported)
	eisa_write(EISA_COMMAND_PORT, 0xFF); // Example: Send a reset command

	// Configure base address registers (if needed)
	eisa_write(EISA_BASE_ADDRESS + 1, 0x0000); // Example: Set base address register
	eisa_write(EISA_BASE_ADDRESS + 2, 0x0000); // Example: Set another base address register

	// Set up interrupts (example)
	eisa_write(EISA_INTERRUPT_CONTROL_PORT, 0x00); // Enable/disable interrupts

	// Configure DMA channels (example)
	eisa_write(EISA_DMA_CHANNEL_CONFIG_PORT, 0x00); // Configure DMA channels
													// Other hardware-specific initialization...
}

void log_error(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

// Detect and configure EISA devices
void eisa_detect_devices()
{
	uint32_t bus, slot, func;
	uint16_t vendor_id, device_id, class_code;
	uint8_t dma_channel;

	for (bus = 0; bus < 256; bus++)
	{
		for (slot = 0; slot < 32; slot++)
		{
			for (func = 0; func < 8; func++)
			{
				uint32_t address = (bus << 16) | (slot << 11) | (func << 8);
				uint32_t id = eisa_read_config_dword(address, 0);
				vendor_id = id & 0xFFFF;
				device_id = (id >> 16) & 0xFFFF;
				class_code = eisa_read_config_word(address, 10);

				if (vendor_id != 0xFFFF)
				{
					if (vendor_id == MY_DEVICE_VENDOR_ID &&
						device_id == MY_DEVICE_DEVICE_ID &&
						class_code == MY_DEVICE_CLASS_CODE)
					{
						uint32_t config1 = eisa_read_config_dword(address, 4);
						uint32_t config2 = eisa_read_config_dword(address, 8);

						if ((config1 & 0x00000010) == 0)
						{
							dma_channel = (config1 >> 4) & 0x03;
							// Replace 0xspecific_port_2 with the actual port number
							eisa_write(0x02, dma_channel); // Example: Assuming 0x02 is the correct port number
							log_error("Failed to enable feature 1");
						}

						if (!dma_channel)
						{
							log_error("Failed to set DMA channel");
						}
						if (!(config2 & 0x00000001))
						{
							log_error("Failed to configure interrupt line");
						}
					}
					else
					{
						log_error("Unknown device detected at address: 0x%04X", address);
					}
				}
				else
				{
					log_error("No device found at address: 0x%04X", address);
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
