#include "fat16_io.h"
#include <stdint.h>
#include "src/kernel/arch/x86/disk/ata.h"
#include "fat16.h"
// I/O port addresses for IDE controller (replace with actual values if needed)
#define PRIMARY_DATA_REGISTER 0x1F0
#define PRIMARY_ERROR_REGISTER 0x1F1
#define PRIMARY_COMMAND_REGISTER 0x1F2
#define PRIMARY_SELECT_REGISTER 0x1F6

// Define bit masks for IDE commands
#define ATA_CMD_READ_SECTORS_WITHOUT_RETRIES 0x20
#define ATA_CMD_READ_SECTORS_WITH_RETRIES 0xC4

// Function to read a sector from disk
int read_sector(uint32_t sector_number, void *buffer)
{
	// 1. Prepare for disk access
	outb(PRIMARY_SELECT_REGISTER, 0x00); // Select primary IDE channel

	// 2. Wait for controller to become ready
	while ((inb(PRIMARY_STATUS_REGISTER) & 0x02) == 0)
	{
	} // Wait for BUSY bit to clear

	// 3. Send read command with parameters
	outb(PRIMARY_COMMAND_REGISTER, ATA_CMD_READ_SECTORS_WITHOUT_RETRIES);  // Replace with retries if needed
	outb(PRIMARY_ERROR_REGISTER, 0);									   // Features (usually set to 0)
	outb(PRIMARY_SELECT_REGISTER, (sector_number & 0x0FF) | 0x80);		   // LBA low byte with LBA bit set
	outb(PRIMARY_COMMAND_REGISTER, ((sector_number >> 8) & 0xFF));		   // LBA mid byte
	outb(PRIMARY_COMMAND_REGISTER, ((sector_number >> 16) & 0x0F) | 0xE0); // LBA high byte with select bit
	outb(PRIMARY_COMMAND_REGISTER, 1);									   // Number of sectors to read (1 in this case)

	// 4. Wait for data transfer to complete (replace with timeout if needed)
	while ((inb(PRIMARY_STATUS_REGISTER) & 0x08) == 0)
	{
	} // Wait for DRQ bit to set

	// 5. Read data from the data register
	for (int i = 0; i < 512; ++i)
	{
		((uint8_t *)buffer)[i] = inb(PRIMARY_DATA_REGISTER);
	}

	// 6. Check for errors (optional, implement error handling)
	if (inb(PRIMARY_STATUS_REGISTER) & 0x01)
	{
		return -1; // Error occurred
	}

	return 0; // Success
}
