#include "memmap.h"

#define BOOTLOADER_MEMMAP_COUNT_ADDR    MEMMAP_BASE
#define BOOTLOADER_MEMMAP_ADDR          (MEMMAP_BASE + 4)

uint32_t get_memory_map(memory_map_entry_t *map, uint32_t max_entries) {
    // Read the number of entries found by the bootloader
    uint32_t entries_found = *(uint32_t*)BOOTLOADER_MEMMAP_COUNT_ADDR;
    memory_map_entry_t *bios_data = (memory_map_entry_t*)BOOTLOADER_MEMMAP_ADDR;

    uint32_t count = 0;
    while (count < entries_found && count < max_entries) {
        map[count] = bios_data[count];
        count++;
    }

    return count;
}
