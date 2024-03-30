#include "pci.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// PCI bus controller base address
#define PCI_BASE_ADDRESS 0x0000

// PCI bus controller data port
#define PCI_DATA_PORT 0x00

// PCI bus controller command port
#define PCI_COMMAND_PORT 0x01

// Initialize the PCI bus
void pci_init()
{
	// Enable PCI bus master
	pci_write(PCI_COMMAND_PORT, 0x04);
}

// Read from a PCI device
uint8_t pci_read_config(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
	uint16_t port = PCI_BASE_ADDRESS | ((uint16_t)bus << 8) | ((uint16_t)device << 3) | ((uint16_t)function << 0x0B) | ((uint16_t)offset & 0xFC);
	uint8_t value;

	// Read from the specified port
	__asm__ volatile("inb %1, %0" : "=a"(value) : "dN"(port));

	return value;
}
// Function to write a value to a PCI configuration register
void pci_write_config(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t value)
{
	// ... (your existing implementation for setting up the port address)

	// Write the specified value to the specified offset
	__asm__ volatile("outb %0, %1" : : "a"(value), "dN"(port));
}

// Set the PCI Latency Timer for a device
void pci_set_latency(uint8_t bus, uint8_t device, uint8_t function, uint8_t value)
{
	pci_write_config(bus, device, function, 0x0D, value);
}

uint16_t pci_read_config_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
	uint16_t value = 0;
	value |= pci_read_config(bus, device, function, offset);
	value |= (uint16_t)pci_read_config(bus, device, function, offset + 1) << 8;
	return value;
}

// Function to read a 32-bit value from PCI configuration space
uint32_t pci_read_config_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
	uint32_t value = 0;
	value |= pci_read_config(bus, device, function, offset);
	value |= (uint32_t)pci_read_config(bus, device, function, offset + 1) << 8;
	value |= (uint32_t)pci_read_config(bus, device, function, offset + 2) << 16;
	value |= (uint32_t)pci_read_config(bus, device, function, offset + 3) << 24;
	return value;
}

// Structure to hold information about a BAR
typedef struct
{
	uint8_t type;  // Memory (0) or I/O (1)
	uint32_t base; // Base address
	uint32_t size; // Size of the region (in bytes)
} bar_t;

// Function to decode a BAR register
bar_t pci_decode_bar(uint32_t bar_value)
{
	bar_t bar;
	bar.type = (bar_value & 1) ? 1 : 0; // Check bit 0 for type

	// Extract address and size information (logic might need adjustment)
	bar.base = bar_value & ~((1 << 0) | (1 << 1)); // Mask out type and prefetchable bits

	// Size calculation needs further processing based on Memory Space Enable bit (bit 3)
	uint32_t size_bits = bar_value & ~(bar.base | (1 << 0) | (1 << 1) | (1 << 3));
	if (size_bits == 0)
	{ // Handle case where size is 0
		bar.size = 0;
	}
	else
	{
		// Calculate size based on the number of leading 1s in size_bits
		int num_leading_ones = 0;
		while (size_bits && (size_bits & 0x80000000))
		{
			num_leading_ones++;
			size_bits <<= 1;
		}
		bar.size = 1 << num_leading_ones;
	}

	return bar;
}
// Detect and configure PCI devices
void pci_detect_devices()
{
	// Scan all PCI buses and devices
	for (int bus = 0; bus < 256; bus++)
	{
		for (int device = 0; device < 32; device++)
		{
			for (int function = 0; function < 8; function++)
			{
				uint16_t vendor_id = pci_read(bus, device, function, 0x00);
				if (vendor_id != 0xFFFF)
				{
					uint16_t device_id = pci_read(bus, device, function, 0x02);

					// Lookup vendor and device ID in a database (e.g., PCI.ids)
					const char *device_name = lookup_device_name(vendor_id, device_id);

					if (device_name != NULL)
					{
						// Device identified!
						printk("Found PCI device: %s (vendor: %04x, device: %04x)\n",
							   device_name, vendor_id, device_id);

						// ... (Optional) Further configuration based on device type
					}
					else
					{
						// Device not found in database, handle unknown device
						printk("Found unknown PCI device (vendor: %04x, device: %04x)\n",
							   vendor_id, device_id);
					}

					// Continue reading other registers, handling BARs, etc.
					uint8_t class_code = pci_read(bus, device, function, 0x0B);
					uint8_t subclass_code = pci_read(bus, device, function, 0x0A);
					uint8_t prog_if = pci_read(bus, device, function, 0x09);
					uint8_t header_type = pci_read(bus, device, function, 0x0E);
					uint8_t irq_line = pci_read(bus, device, function, 0x3C);

					// ... (rest of your code for handling BARs, etc.)
				}
			}
		}
	}
}

// Read from a PCI device
uint8_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
	uint16_t port = PCI_BASE_ADDRESS | ((uint16_t)bus << 8) | ((uint16_t)device << 3) | ((uint16_t)function << 0x0B) | ((uint16_t)offset & 0xFC);
	uint8_t value;

	// Read from the specified port
	__asm__ volatile("inb %1, %0" : "=a"(value) : "dN"(port));

	return value;
}

// Write to a PCI device
void pci_write(uint16_t port, uint8_t value)
{
	// Write the specified value to the specified port
	__asm__ volatile("outb %0, %1" : : "a"(value), "dN"(port));
}
