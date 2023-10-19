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

// Detect and configure PCI devices
void pci_detect_devices()
{
    // Scan all PCI buses and devices
    for (int bus = 0; bus < 256; bus++) {
        for (int device = 0; device < 32; device++) {
            for (int function = 0; function < 8; function++) {
                uint16_t vendor_id = pci_read(bus, device, function, 0x00);
                if (vendor_id != 0xFFFF) {
                    uint16_t device_id = pci_read(bus, device, function, 0x02);
                    uint8_t class_code = pci_read(bus, device, function, 0x0B);
                    uint8_t subclass_code = pci_read(bus, device, function, 0x0A);
                    uint8_t prog_if = pci_read(bus, device, function, 0x09);
                    uint8_t header_type = pci_read(bus, device, function, 0x0E);
                    uint8_t irq_line = pci_read(bus, device, function, 0x3C);
                    uint32_t bar0 = pci_read(bus, device, function, 0x10);
                    uint32_t bar1 = pci_read(bus, device, function, 0x14);
                    uint32_t bar2 = pci_read(bus, device, function, 0x18);
                    uint32_t bar3 = pci_read(bus, device, function, 0x1C);
                    uint32_t bar4 = pci_read(bus, device, function, 0x20);
                    uint32_t bar5 = pci_read(bus, device, function, 0x24);
                    // Add any necessary device detection and configuration code here
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
