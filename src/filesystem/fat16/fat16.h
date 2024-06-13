#ifndef FAT16_H
#define FAT16_H

#include <stdint.h>

#include "../../ata.h"

// Define constants for sector size, cluster size, etc. (replace with actual values)
#define SECTOR_SIZE 512
#define BYTES_PER_CLUSTER 4096 // Example: 8 sectors per cluster

// Define structures for FAT entry, directory entry, etc.
typedef struct
{
	// ... (FAT entry fields) ...
} fat_entry_t;

typedef struct
{
	// ... (Directory entry fields) ...
} directory_entry_t;

// Function prototypes for FAT operations
int read_sector(uint32_t sector_number, void *buffer);
int write_sector(uint32_t sector_number, void *buffer);
int mount_fat16(const char *device_name); // Mount the FAT16 volume
// ... (other function prototypes) ...

#endif
