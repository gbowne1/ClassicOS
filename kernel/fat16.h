#ifndef FAT16_H
#define FAT16_H

#include <stdint.h>
#include <stdbool.h>

/* FAT16 Constants */
#define FAT16_SECTOR_SIZE        512
#define FAT16_CLUSTER_SIZE       1
#define FAT16_MAX_FILENAME_LEN   11   // 8.3 format
#define FAT16_ROOT_DIR_ENTRIES   224  // Fat16 root directory entries (typically 512 bytes per entry)

#define FAT16_BOOT_SECTOR        0
#define FAT16_FAT1_SECTOR        1
#define FAT16_FAT2_SECTOR        2
#define FAT16_ROOT_DIR_SECTOR    19  // First sector of root directory

/* Boot Sector */
typedef struct {
    uint8_t jmp[3];               // Jump instruction to code
    uint8_t oem_name[8];          // OEM Name
    uint16_t bytes_per_sector;    // Bytes per sector (512)
    uint8_t sectors_per_cluster;  // Sectors per cluster
    uint16_t reserved_sectors;    // Reserved sectors
    uint8_t num_fats;             // Number of FAT tables
    uint16_t max_root_entries;    // Max number of root directory entries
    uint16_t total_sectors_16;    // Total sectors in FAT16
    uint8_t media_type;           // Media type (0xF8 = fixed drive)
    uint16_t sectors_per_fat;    // Sectors per FAT table
    uint16_t sectors_per_track;  // Sectors per track (for CHS addressing)
    uint16_t num_heads;          // Number of heads (for CHS addressing)
    uint32_t hidden_sectors;     // Hidden sectors (before the partition)
    uint32_t total_sectors_32;   // Total sectors in FAT16 (extended)
} __attribute__((packed)) fat16_boot_sector_t;

/* FAT16 Directory Entry */
typedef struct {
    uint8_t name[8];   // File name (8 chars)
    uint8_t ext[3];    // File extension (3 chars)
    uint8_t attributes; // File attributes (e.g., directory, read-only)
    uint8_t reserved;   // Reserved
    uint8_t creation_time[2]; // Creation time
    uint8_t creation_date[2]; // Creation date
    uint8_t last_access_date[2]; // Last access date
    uint8_t first_cluster_high[2]; // High part of first cluster number
    uint8_t last_mod_time[2];  // Last modification time
    uint8_t last_mod_date[2];  // Last modification date
    uint8_t first_cluster_low[2];  // Low part of first cluster number
    uint32_t file_size;         // File size in bytes
} __attribute__((packed)) fat16_dir_entry_t;

/* Function Prototypes */
bool mount_fat16(void);
bool read_sector(uint32_t lba, uint8_t* buffer);
bool write_sector(uint32_t lba, const uint8_t* buffer);
bool parse_fat16_boot_sector(void);
bool parse_fat16_root_dir(void);
bool read_fat16_entry(uint16_t entry_index, fat16_dir_entry_t* entry);

#endif // FAT16_H
