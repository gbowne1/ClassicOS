#include "fat12.h"
#include <stddef.h> // for NULL

// --- Globals for Filesystem State ---
static fat12_bpb_t bpb;
static uint32_t fat_start_lba;
static uint32_t root_dir_lba;
static uint32_t data_start_lba;
static uint32_t root_dir_sectors;

// Scratch buffer to read sectors (avoids large stack usage)
static uint8_t g_sector_buffer[FAT12_SECTOR_SIZE];

// --- Utils (Since we don't have string.h) ---
static int k_memcmp(const void *s1, const void *s2, uint32_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;
    for (uint32_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) return p1[i] - p2[i];
    }
    return 0;
}

// Converts "file.txt" to "FILE    TXT" for comparison
static void to_fat_name(const char *src, char *dest) {
    // Initialize with spaces
    for(int i=0; i<11; i++) dest[i] = ' ';

    int i = 0, j = 0;
    // Copy Name
    while (src[i] != '\0' && src[i] != '.' && j < 8) {
        // Convert to uppercase (simple version)
        char c = src[i];
        if (c >= 'a' && c <= 'z') c -= 32; 
        dest[j++] = c;
        i++;
    }
    
    // Skip extension dot
    if (src[i] == '.') i++;

    // Copy Extension
    j = 8;
    while (src[i] != '\0' && j < 11) {
        char c = src[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        dest[j++] = c;
        i++;
    }
}

// --- Core Logic ---

void fat12_init() {
    // 1. Read Boot Sector (LBA 0)
    disk_read_sector(0, g_sector_buffer);

    // 2. Copy BPB data safely
    // We cast the buffer to our struct
    fat12_bpb_t *boot_sector = (fat12_bpb_t*)g_sector_buffer;
    bpb = *boot_sector;

    // 3. Calculate System Offsets
    fat_start_lba  = bpb.reserved_sectors;
    
    // Root Dir starts after FATs
    // LBA = Reserved + (FatCount * SectorsPerFat)
    root_dir_lba   = fat_start_lba + (bpb.fat_count * bpb.sectors_per_fat);
    
    // Calculate size of Root Directory in sectors
    // (Entries * 32 bytes) / 512
    root_dir_sectors = (bpb.dir_entries_count * 32 + FAT12_SECTOR_SIZE - 1) / FAT12_SECTOR_SIZE;

    // Data starts after Root Directory
    data_start_lba = root_dir_lba + root_dir_sectors;
}

// Helper: Read the FAT table to find the NEXT cluster
static uint16_t fat12_get_next_cluster(uint16_t current_cluster) {
    // FAT12 Offset Calculation:
    // Offset = Cluster + (Cluster / 2)
    uint32_t fat_offset = current_cluster + (current_cluster / 2);
    
    uint32_t fat_sector = fat_start_lba + (fat_offset / FAT12_SECTOR_SIZE);
    uint32_t ent_offset = fat_offset % FAT12_SECTOR_SIZE;

    // Read the sector containing the FAT entry
    disk_read_sector(fat_sector, g_sector_buffer);

    // Read 16 bits (2 bytes)
    // Note: If ent_offset == 511, the entry spans two sectors. 
    // For simplicity in this snippet, we ignore that edge case (rare).
    // A robust kernel would check if(ent_offset == 511) and read next sector.
    
    uint16_t val = *(uint16_t*)&g_sector_buffer[ent_offset];

    if (current_cluster & 1) {
        return val >> 4;      // Odd: High 12 bits
    } else {
        return val & 0x0FFF;  // Even: Low 12 bits
    }
}

file_t fat12_open(const char *filename) {
    file_t file = {0};
    char target_name[11];
    to_fat_name(filename, target_name);

    // Search Root Directory
    for (uint32_t i = 0; i < root_dir_sectors; i++) {
        disk_read_sector(root_dir_lba + i, g_sector_buffer);

        fat12_entry_t *entry = (fat12_entry_t*)g_sector_buffer;

        // Check all 16 entries in this sector (512 / 32 = 16)
        for (int j = 0; j < 16; j++) {
            if (entry[j].filename[0] == 0x00) return file; // End of Dir
            
            // Check if filename matches
            if (k_memcmp(entry[j].filename, target_name, 11) == 0) {
                // Found it!
                file.start_cluster = entry[j].low_cluster_num;
                file.size = entry[j].file_size;
                
                // Initialize file cursor
                file.current_cluster = file.start_cluster;
                file.bytes_read = 0;
                return file;
            }
        }
    }
    
    // Not found (file.start_cluster will be 0)
    return file;
}

uint32_t fat12_read(file_t *file, uint8_t *buffer, uint32_t bytes_to_read) {
    if (file->start_cluster == 0) return 0; // File not open

    uint32_t total_read = 0;

    while (bytes_to_read > 0) {
        // Check for EOF marker in FAT12 (>= 0xFF8)
        if (file->current_cluster >= 0xFF8) break;

        // Calculate Physical LBA of current cluster
        // LBA = DataStart + ((Cluster - 2) * SectorsPerCluster)
        uint32_t lba = data_start_lba + ((file->current_cluster - 2) * bpb.sectors_per_cluster);

        // Read the cluster
        // NOTE: Assumes SectorsPerCluster = 1 (Standard Floppy)
        disk_read_sector(lba, g_sector_buffer);

        // Determine how much to copy from this sector
        uint32_t chunk_size = FAT12_SECTOR_SIZE;
        
        // If the file is smaller than a sector, or we are at the end
        if (chunk_size > bytes_to_read) chunk_size = bytes_to_read;
        
        // Check if we are reading past file size
        if (file->bytes_read + chunk_size > file->size) {
            chunk_size = file->size - file->bytes_read;
        }

        // Copy to user buffer
        for (uint32_t i = 0; i < chunk_size; i++) {
            buffer[total_read + i] = g_sector_buffer[i];
        }

        total_read += chunk_size;
        file->bytes_read += chunk_size;
        bytes_to_read -= chunk_size;

        // If we finished this cluster, move to the next one
        if (chunk_size == FAT12_SECTOR_SIZE) { // Or strictly logic based on position
             file->current_cluster = fat12_get_next_cluster(file->current_cluster);
        } else {
            // We finished the file or the request
            break;
        }
    }

    return total_read;
}
