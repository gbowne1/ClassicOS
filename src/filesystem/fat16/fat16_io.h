#ifndef FAT16_IO_H
#define FAT16_IO_H

#include <stdint.h>
#include "fat16.h"
#include "../../kernel/arch/x86/disk/ata.h"

// I/O port addresses for IDE controller (replace with actual values if needed)
#define PRIMARY_DATA_REGISTER 0x1F0
#define PRIMARY_ERROR_REGISTER 0x1F1
#define PRIMARY_COMMAND_REGISTER 0x1F2
#define PRIMARY_SELECT_REGISTER 0x1F6

// Define bit masks for IDE commands
#define ATA_CMD_READ_SECTORS_WITHOUT_RETRIES 0x20
#define ATA_CMD_READ_SECTORS_WITH_RETRIES 0xC4

// Function prototype for reading a sector from disk
int read_sector(uint32_t sector_number, void *buffer);

#endif // FAT16_IO_H
