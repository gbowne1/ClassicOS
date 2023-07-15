#ifndef PCI_H
#define PCI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the PCI bus
void pci_init();

// Detect and configure PCI devices
void pci_detect_devices();

// Read from a PCI device
uint8_t pci_read(uint16_t port);

// Write to a PCI device
void pci_write(uint16_t port, uint8_t value);

#endif