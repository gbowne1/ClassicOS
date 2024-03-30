#include "ahci.h"
#include <stdint.h>

// Function prototypes (implementations below)
uint32_t ahci_read_reg(uint32_t offset);
void ahci_write_reg(uint32_t offset, uint32_t value);

// Function to send an AHCI command
int ahci_send_command(uint32_t cmd_slot, ahci_cmd_t *cmd);

// ... (Add other AHCI functions for initialization, status checks, etc.)

// Helper function to read a 32-bit register
uint32_t ahci_read_reg(uint32_t offset)
{
	return *(volatile uint32_t *)(HBA_PORT_BASE + offset);
}

// Helper function to write a 32-bit register
void ahci_write_reg(uint32_t offset, uint32_t value)
{
	*(volatile uint32_t *)(HBA_PORT_BASE + offset) = value;
}

// Function to send an AHCI command
int ahci_send_command(uint32_t cmd_slot, ahci_cmd_t *cmd)
{
	// Implement logic to write the command list entry to appropriate registers
	// ...

	// Wait for command completion (check relevant AHCI registers)
	// ...

	// Return status based on completion and error checking
	// ...
}
