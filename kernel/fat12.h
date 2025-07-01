#ifndef FAT12_H
#define FAT12_H

#include <stdint.h> /* Include standard integer types */
#include <stdio.h>  /* Include standard I/O library */
#include <stdlib.h> /* Include standard library */

#define FAT12_SECTOR_SIZE 512 /* Sector size for FAT12 */
#define FAT12_MAX_FILES 128 /* Maximum number of files in root directory */
#define FAT12_ROOT_DIR_SECTORS 1 /* Number of sectors for root directory */

typedef struct {
    uint8_t jump[3]; /* Jump instruction for boot */
    char oem[8]; /* OEM name */
    uint16_t bytes_per_sector; /* Bytes per sector */
    uint8_t sectors_per_cluster; /* Sectors per cluster */
    uint16_t reserved_sectors; /* Reserved sectors count */
    uint8_t num_fats; /* Number of FATs */
    uint16_t max_root_dir_entries; /* Max entries in root directory */
    uint16_t total_sectors; /* Total sectors */
    uint8_t media_descriptor; /* Media descriptor */
    uint16_t fat_size; /* Size of each FAT */
    uint16_t sectors_per_track; /* Sectors per track */
    uint16_t num_heads; /* Number of heads */
    uint32_t hidden_sectors; /* Hidden sectors count */
    uint32_t total_sectors_large; /* Total sectors for large disks */
} __attribute__((packed)) FAT12_BootSector; /* Packed structure for boot sector */

typedef struct {
    char name[11]; /* File name (8.3 format) */
    uint8_t attr; /* File attributes */
    uint16_t reserved; /* Reserved */
    uint16_t time; /* Time of last write */
    uint16_t date; /* Date of last write */
    uint16_t start_cluster; /* Starting cluster number */
    uint32_t file_size; /* File size in bytes */
} __attribute__((packed)) FAT12_DirEntry; /* Directory entry structure */

void initialize_fat12(const char *disk_image); /* Function to initialize FAT12 */
void read_fat12(const char *disk_image); /* Function to read FAT12 */
void write_fat12(const char *disk_image); /* Function to write FAT12 */
void list_files(const char *disk_image); /* Function to list files in root directory */
void read_file(const char *disk_image, const char *filename); /* Function to read a file */
void write_file(const char *disk_image, const char *filename, const uint8_t *data, size_t size); /* Function to write a file */

#endif 
/* FAT12_H */
