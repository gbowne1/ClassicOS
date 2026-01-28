#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stddef.h>
#include "memmap.h"
#include "paging.h" // For PAGE_SIZE

#define BLOCKS_PER_BYTE 8

void pmm_init(memory_map_entry_t* mmap, uint32_t mmap_size, uintptr_t bitmap_addr);
void pmm_mark_used(uintptr_t addr);
void pmm_mark_free(uintptr_t addr);

void* pmm_alloc_block();
void  pmm_free_block(void* addr);

uint32_t pmm_get_used_block_count();
uint32_t pmm_get_free_block_count();

#endif
