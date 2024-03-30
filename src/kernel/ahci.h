#ifndef AHCI_H
#define AHCI_H

#include <stdint.h>

// Base address of AHCI registers (replace with actual value)
#define HBA_PORT_BASE 0x10000000

// AHCI register offsets
#define HBA_CAP 0x00	 // Host Capabilities
#define HBA_PI 0x0C		 // Port Interrupt
#define HBA_ISR 0x20	 // Interrupt Status
#define HBA_IMR 0x30	 // Interrupt Mask
#define HBA_CMD 0x34	 // Command List Head
#define HBA_CMD_SLOT_0 0 // First command slot

// AHCI command list structure
typedef struct
{
	uint32_t cmd_sts;	  // FIS (Frame in flight)
	uint32_t prdt_addr;	  // Physical address of PRDT (SATA FIS)
	uint32_t prdt_length; // Length of PRDT in bytes
	uint32_t reserved[4]; // Reserved (set to 0)
} __attribute__((packed)) ahci_cmd_t;

// ... (Add other AHCI register definitions and structures as needed)

#endif /* AHCI_H */
