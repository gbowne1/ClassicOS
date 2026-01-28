#ifndef MEMMAP_H
#define MEMMAP_H

#include <stdint.h>

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t ext;
} __attribute__((packed)) memory_map_entry_t;

uint32_t get_memory_map(memory_map_entry_t *map, uint32_t max_entries);

#endif
