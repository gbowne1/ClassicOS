#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>

// --- Configuration ---
#define FAT12_SECTOR_SIZE 512

// --- On-Disk Structures (Must be Packed) ---

// BIOS Parameter Block (Start of Boot Sector)
typedef struct {
    uint8_t  jump[3];
    char     oem[8];
    uint16_t bytes_per_sector;    // 512
    uint8_t  sectors_per_cluster; // 1
    uint16_t reserved_sectors;    // 1 (Boot sector)
    uint8_t  fat_count;           // 2
    uint16_t dir_entries_count;   // 224
    uint16_t total_sectors;       // 2880
    uint8_t  media_descriptor;    // 0xF0
    uint16_t sectors_per_fat;     // 9
    uint16_t sectors_per_track;   // 18
    uint16_t heads;               // 2
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
} __attribute__((packed)) fat12_bpb_t;

// Directory Entry (32 bytes)
typedef struct {
    char     filename[8];
    char     ext[3];
    uint8_t  attributes;
    uint8_t  reserved;
    uint8_t  creation_ms;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t high_cluster_num; // Always 0 in FAT12
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t low_cluster_num;  // The starting cluster
    uint32_t file_size;        // Size in bytes
} __attribute__((packed)) fat12_entry_t;

// --- Kernel File Handle ---
// This is what your kernel uses to track an open file
typedef struct {
    char     name[11];
    uint32_t size;
    uint16_t start_cluster;
    uint16_t current_cluster;
    uint32_t current_sector_in_cluster;
    uint32_t bytes_read;
} file_t;

// --- Public API ---

// You must implement this in your disk driver (e.g., floppy.c)
// Returns 0 on success, non-zero on error.
int disk_read_sector(uint32_t lba, uint8_t *buffer);

void fat12_init();
file_t fat12_open(const char *filename);
uint32_t fat12_read(file_t *file, uint8_t *buffer, uint32_t bytes_to_read);

#endif // FAT12_H
