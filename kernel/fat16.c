#include "fat16.h"
#include "ata.h"   // Use ata_read_sector and ata_write_sector
#include "print.h" // For debugging
#include <string.h> // For string manipulation

// Global variables
static fat16_boot_sector_t boot_sector;
static uint32_t root_dir_sector = FAT16_ROOT_DIR_SECTOR;

// Read a sector from the disk using ATA
bool read_sector(uint32_t lba, uint8_t* buffer) {
    return ata_read_sector(lba, buffer);
}

// Write a sector to the disk using ATA
bool write_sector(uint32_t lba, const uint8_t* buffer) {
    return ata_write_sector(lba, buffer);
}

// Parse the boot sector to retrieve basic file system info
bool parse_fat16_boot_sector(void) {
    uint8_t sector_buffer[FAT16_SECTOR_SIZE];

    // Read the boot sector
    if (!read_sector(FAT16_BOOT_SECTOR, sector_buffer)) {
        print_string("[FAT16] Failed to read boot sector\n");
        return false;
    }

    // Cast to boot sector structure
    memcpy(&boot_sector, sector_buffer, sizeof(fat16_boot_sector_t));

    // Check for FAT16 signature
    if (boot_sector.oem_name[0] != 'F' || boot_sector.oem_name[1] != 'A' || boot_sector.oem_name[2] != 'T') {
        print_string("[FAT16] Invalid FAT16 boot sector signature\n");
        return false;
    }

    print_string("[FAT16] FAT16 boot sector parsed successfully\n");
    return true;
}

// Parse the root directory
bool parse_fat16_root_dir(void) {
    uint8_t sector_buffer[FAT16_SECTOR_SIZE];

    for (int i = 0; i < (boot_sector.max_root_entries / (FAT16_SECTOR_SIZE / sizeof(fat16_dir_entry_t))); i++) {
        // Read root directory sector
        if (!read_sector(root_dir_sector + i, sector_buffer)) {
            print_string("[FAT16] Failed to read root directory sector\n");
            return false;
        }

        // Parse the root directory entries
        for (int j = 0; j < (FAT16_SECTOR_SIZE / sizeof(fat16_dir_entry_t)); j++) {
            fat16_dir_entry_t* entry = (fat16_dir_entry_t*)&sector_buffer[j * sizeof(fat16_dir_entry_t)];
            if (entry->name[0] == 0x00) {
                // End of directory entries
                return true;
            }
            if (entry->name[0] != 0xE5) {
                // Print file name (8.3 format)
                char filename[12];
                strncpy(filename, (char*)entry->name, 8);
                filename[8] = '.';
                strncpy(&filename[9], (char*)entry->ext, 3);
                filename[11] = '\0';
                print_string(filename);
                print_string("\n");
            }
        }
    }
    return true;
}

// Read a specific directory entry from the FAT16 root directory
bool read_fat16_entry(uint16_t entry_index, fat16_dir_entry_t* entry) {
    uint8_t sector_buffer[FAT16_SECTOR_SIZE];
    uint32_t sector_num = FAT16_ROOT_DIR_SECTOR + (entry_index / (FAT16_SECTOR_SIZE / sizeof(fat16_dir_entry_t)));
    uint16_t entry_offset = entry_index % (FAT16_SECTOR_SIZE / sizeof(fat16_dir_entry_t));

    // Read the sector
    if (!read_sector(sector_num, sector_buffer)) {
        print_string("[FAT16] Failed to read root directory sector\n");
        return false;
    }

    // Get the entry
    memcpy(entry, &sector_buffer[entry_offset * sizeof(fat16_dir_entry_t)], sizeof(fat16_dir_entry_t));
    return true;
}

// Mount the FAT16 filesystem
bool mount_fat16(void) {
    // Parse the boot sector
    if (!parse_fat16_boot_sector()) {
        return false;
    }

    // Parse the root directory
    if (!parse_fat16_root_dir()) {
        return false;
    }

    print_string("[FAT16] Filesystem mounted successfully\n");
    return true;
}
