#ifndef ATA_H
#define ATA_H

#include "../include/types.h"
// I/O port addresses for IDE controllers (replace with actual values if needed)
#define PRIMARY_DATA_REGISTER 0x1F0
#define PRIMARY_ERROR_REGISTER 0x1F1
#define PRIMARY_COMMAND_REGISTER 0x1F2
#define PRIMARY_SELECT_REGISTER 0x1F6
#define SECONDARY_DATA_REGISTER 0x1F4
#define SECONDARY_ERROR_REGISTER 0x1F5
#define SECONDARY_COMMAND_REGISTER 0x1F6
#define SECONDARY_SELECT_REGISTER 0x1F7

// Define bit masks for IDE commands (replace with actual values if needed)
#define ATA_CMD_READ_SECTORS_WITHOUT_RETRIES 0x20
#define ATA_CMD_READ_SECTORS_WITH_RETRIES 0xC4
#define ATA_CMD_WRITE_SECTORS_WITHOUT_RETRIES 0x30
#define ATA_CMD_WRITE_SECTORS_WITH_RETRIES 0x34

// Define bit masks for IDE status register bits (replace with actual values if needed)
#define ATA_STATUS_BSY 0x02  // Busy bit
#define ATA_STATUS_DRQ 0x08  // Data Request bit
#define ATA_STATUS_ERR 0x01  // Error bit

// Function prototypes (replace with actual function implementations)
int read_sector(uint32_t sector_number, void *buffer, int drive); // Read a sector from disk (drive: 0 for primary, 1 for secondary)
int write_sector(uint32_t sector_number, void *buffer, int drive); // Write a sector to disk (drive: 0 for primary, 1 for secondary)

// Additional function prototypes specific to your IDE controller library can be added here

#endif
