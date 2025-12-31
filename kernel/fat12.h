#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>
#include <stdbool.h>

#define FAT12_SECTOR_SIZE 512

/* --- On-Disk Structures --- */

typedef struct {
    uint8_t  jump[3];
    char     oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  fat_count;
    uint16_t dir_entries_count;
    uint16_t total_sectors;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
} __attribute__((packed)) fat12_bpb_t;

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
    uint16_t low_cluster_num;
    uint32_t file_size;
} __attribute__((packed)) fat12_entry_t;

/* --- Kernel File Handle --- */

typedef struct {
    uint32_t size;
    uint16_t start_cluster;
    uint16_t current_cluster;
    uint32_t bytes_read;
    bool     valid;
} file_t;

/* --- API --- */

void     fat12_init(void);
file_t   fat12_open(const char *filename);
uint32_t fat12_read(file_t *file, uint8_t *buffer, uint32_t bytes_to_read);

#endif
