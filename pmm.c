#include "pmm.h"
#include "memory.h" // For memset

static uint32_t* pmm_bitmap = NULL;
static uint32_t  max_blocks = 0;
static uint32_t  used_blocks = 0;

// Internal bitmap helpers
static inline void bitmap_set(uint32_t bit) {
    pmm_bitmap[bit / 32] |= (1 << (bit % 32));
}

static inline void bitmap_unset(uint32_t bit) {
    pmm_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static inline int bitmap_test(uint32_t bit) {
    return pmm_bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_init(memory_map_entry_t* mmap, uint32_t mmap_size, uintptr_t bitmap_addr) {
    // 1. Calculate total memory from mmap to find max_blocks
    uint64_t total_mem = 0;
    for (uint32_t i = 0; i < mmap_size; i++) {
        if (mmap[i].type == 1) { // Available RAM
            total_mem = mmap[i].base_addr + mmap[i].length;
        }
    }

    max_blocks = (uint32_t)(total_mem / PAGE_SIZE);
    used_blocks = max_blocks; 
    pmm_bitmap = (uint32_t*)bitmap_addr;

    // 2. Default all memory to "Reserved" (1s)
    memset(pmm_bitmap, 0xFF, max_blocks / BLOCKS_PER_BYTE);

    // 3. Mark only the regions reported as Type 1 (Available) as free (0s)
    for (uint32_t i = 0; i < mmap_size; i++) {
        if (mmap[i].type == 1) {
            uint32_t start_block = (uint32_t)(mmap[i].base_addr / PAGE_SIZE);
            uint32_t block_count = (uint32_t)(mmap[i].length / PAGE_SIZE);
            
            for (uint32_t j = 0; j < block_count; j++) {
                bitmap_unset(start_block + j);
                used_blocks--;
            }
        }
    }

    // 4. Critical: Re-protect the first 1MB (BIOS/VGA/Real Mode stuff)
    for (uint32_t i = 0; i < (1024 * 1024) / PAGE_SIZE; i++) {
        pmm_mark_used(i * PAGE_SIZE);
    }

    // 5. Critical: Re-protect the Kernel + Page Tables
    // Since your paging tables are at 0x200000 and linker at 1MB, 
    // mark everything from 0x100000 to roughly 0x400000 as used for safety.
    for (uint32_t i = 0x100000 / PAGE_SIZE; i < 0x400000 / PAGE_SIZE; i++) {
        pmm_mark_used(i * PAGE_SIZE);
    }
    
    // 6. Protect the bitmap itself
    uint32_t bitmap_size_blocks = (max_blocks / BLOCKS_PER_BYTE) / PAGE_SIZE + 1;
    for(uint32_t i = 0; i < bitmap_size_blocks; i++) {
        pmm_mark_used(bitmap_addr + (i * PAGE_SIZE));
    }
}

void pmm_mark_used(uintptr_t addr) {
    uint32_t block = addr / PAGE_SIZE;
    if (!bitmap_test(block)) {
        bitmap_set(block);
        used_blocks++;
    }
}

void pmm_mark_free(uintptr_t addr) {
    uint32_t block = addr / PAGE_SIZE;
    if (bitmap_test(block)) {
        bitmap_unset(block);
        used_blocks--;
    }
}

void* pmm_alloc_block() {
    for (uint32_t i = 0; i < max_blocks / 32; i++) {
        if (pmm_bitmap[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if (!bitmap_test(i * 32 + j)) {
                    uint32_t addr = (i * 32 + j) * PAGE_SIZE;
                    pmm_mark_used(addr);
                    return (void*)addr;
                }
            }
        }
    }
    return NULL; // OOM
}

void pmm_free_block(void* addr) {
    pmm_mark_free((uintptr_t)addr);
}
