#include "memmap.h"

uint32_t get_memory_map(memory_map_entry_t *map, uint32_t max_entries) {
    uint32_t count = 0;

    if (max_entries >= 1) {
        map[count].base_addr = 0x00000000;
        map[count].length = 0x0009FC00;
        map[count].type = 1;
        count++;
    }

    if (max_entries >= 2) {
        map[count].base_addr = 0x00100000;
        map[count].length = 0x1FF00000;
        map[count].type = 1;
        count++;
    }

    return count;
}