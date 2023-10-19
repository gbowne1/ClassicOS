#ifndef PCI_H
#define PCI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the PCI bus
void pci_init();

// Detect and configure PCI devices
void pci_detect_devices();

uint8_t pci_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

// Write to a PCI device
void pci_write(uint16_t port, uint8_t value);

#endif