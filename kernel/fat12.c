#include "fat12.h"
#include "floppy.h"
#include <stddef.h>

static fat12_bpb_t bpb;
static uint32_t fat_start_lba;
static uint32_t root_dir_lba;
static uint32_t data_start_lba;
static uint32_t root_dir_sectors;

// Local scratch buffer
static uint8_t sector_buffer[FAT12_SECTOR_SIZE];

/* --- Internal Helpers --- */

static int k_memcmp(const void *s1, const void *s2, uint32_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (uint32_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            // Correct way to return the difference:
            // If p1[i] > p2[i], returns positive.
            // If p1[i] < p2[i], returns negative.
            return (int)p1[i] - (int)p2[i];
        }
    }

    return 0;
}

static void to_fat_name(const char *src, char *dest) {
    for (int i = 0; i < 11; i++) dest[i] = ' ';
    int i = 0, j = 0;
    while (src[i] && src[i] != '.' && j < 8) {
        char c = src[i++];
        dest[j++] = (c >= 'a' && c <= 'z') ? c - 32 : c;
    }
    if (src[i] == '.') i++;
    j = 8;
    while (src[i] && j < 11) {
        char c = src[i++];
        dest[j++] = (c >= 'a' && c <= 'z') ? c - 32 : c;
    }
}

/* --- FAT Chain Logic --- */



static uint16_t fat12_get_next_cluster(uint16_t cluster) {
    uint32_t fat_offset = cluster + (cluster / 2);
    uint32_t fat_sector = fat_start_lba + (fat_offset / FAT12_SECTOR_SIZE);
    uint32_t ent_offset = fat_offset % FAT12_SECTOR_SIZE;

    uint8_t bytes[2];
    floppy_read_sector(fat_sector, sector_buffer);
    bytes[0] = sector_buffer[ent_offset];

    // Boundary Fix: If entry spans two sectors
    if (ent_offset == 511) {
        floppy_read_sector(fat_sector + 1, sector_buffer);
        bytes[1] = sector_buffer[0];
    } else {
        bytes[1] = sector_buffer[ent_offset + 1];
    }

    uint16_t val = (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8);
    return (cluster & 1) ? (val >> 4) : (val & 0x0FFF);
}

/* --- Public API Implementation --- */

void fat12_init(void) {
    floppy_read_sector(0, sector_buffer);
    bpb = *(fat12_bpb_t *)sector_buffer;

    fat_start_lba = bpb.reserved_sectors;
    root_dir_lba  = fat_start_lba + (bpb.fat_count * bpb.sectors_per_fat);
    root_dir_sectors = (bpb.dir_entries_count * 32 + 511) / 512;
    data_start_lba = root_dir_lba + root_dir_sectors;
}

file_t fat12_open(const char *filename) {
    file_t file = {0};
    char fat_name[11];
    to_fat_name(filename, fat_name);

    for (uint32_t i = 0; i < root_dir_sectors; i++) {
        floppy_read_sector(root_dir_lba + i, sector_buffer);
        fat12_entry_t *entries = (fat12_entry_t *)sector_buffer;

        for (int j = 0; j < 16; j++) {
            if (entries[j].filename[0] == 0x00) return file; // End of list
            if ((uint8_t)entries[j].filename[0] == 0xE5) continue; // Deleted

            if (k_memcmp(entries[j].filename, fat_name, 11) == 0) {
                file.size = entries[j].file_size;
                file.start_cluster = entries[j].low_cluster_num;
                file.current_cluster = file.start_cluster;
                file.bytes_read = 0;
                file.valid = true;
                return file;
            }
        }
    }
    return file;
}

uint32_t fat12_read(file_t *file, uint8_t *buffer, uint32_t count) {
    if (!file->valid || file->current_cluster >= 0xFF8) return 0;

    uint32_t total_read = 0;
    uint32_t cluster_size = bpb.sectors_per_cluster * FAT12_SECTOR_SIZE;

    while (total_read < count && file->current_cluster < 0xFF8) {
        uint32_t lba = data_start_lba + (file->current_cluster - 2) * bpb.sectors_per_cluster;
        
        // Read each sector in the cluster
        for (uint8_t s = 0; s < bpb.sectors_per_cluster; s++) {
            floppy_read_sector(lba + s, sector_buffer);

            // Calculate how much of this sector we actually need
            uint32_t offset_in_sector = file->bytes_read % FAT12_SECTOR_SIZE;
            uint32_t left_in_sector = FAT12_SECTOR_SIZE - offset_in_sector;
            uint32_t left_in_file = file->size - file->bytes_read;
            uint32_t left_to_request = count - total_read;

            uint32_t chunk = left_in_sector;
            if (chunk > left_in_file)    chunk = left_in_file;
            if (chunk > left_to_request) chunk = left_to_request;

            // Simple memcpy replacement
            for (uint32_t i = 0; i < chunk; i++) {
                buffer[total_read + i] = sector_buffer[offset_in_sector + i];
            }

            total_read += chunk;
            file->bytes_read += chunk;

            if (chunk == 0 || file->bytes_read >= file->size || total_read >= count) break;
        }

        // If we've finished the cluster, move to next
        if (file->bytes_read % cluster_size == 0 || file->bytes_read >= file->size) {
             if (file->bytes_read < file->size) {
                file->current_cluster = fat12_get_next_cluster(file->current_cluster);
             }
        }
        
        if (file->bytes_read >= file->size) break;
    }

    return total_read;
}

int disk_read_sector(uint32_t lba, uint8_t *buffer) {
    // Convert LBA to CHS (Cylinder-Head-Sector) for older BIOS calls
    // Note: Standard 1.44MB Floppy geometry: 18 sectors per track, 2 heads
    uint32_t sector = (lba % 18) + 1;
    uint32_t head   = (lba / 18) % 2;
    uint32_t cylinder = (lba / (18 * 2));

    uint8_t error_code;
    uint8_t success;

    __asm__ __volatile__ (
        "int $0x13"
        : "=a"(error_code), "=c"(success)
        : "a"(0x0201),             // AH=02 (Read), AL=01 (1 sector)
          "b"(buffer),             // EBX = buffer address
          "c"((cylinder << 8) | sector), // CH = Cyl, CL = Sector
          "d"((head << 8) | 0)     // DH = Head, DL = Drive 0 (A:)
        : "memory"
    );

    return (error_code == 0) ? 0 : -1;
}
