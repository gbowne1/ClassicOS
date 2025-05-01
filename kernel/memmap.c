#include "memmap.h"

uint32_t get_memory_map(memory_map_entry_t *map, uint32_t max_entries) {
    // Fill with dummy values for now
    map[0].base_addr = 0x00000000;
    map[0].length = 0x0009FC00;
    map[0].type = 1;

    map[1].base_addr = 0x00100000;
    map[1].length = 0x1FF00000;
    map[1].type = 1;

    return 2; // 2 regions
}
