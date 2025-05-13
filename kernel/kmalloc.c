#include "kmalloc.h"
#include "terminal.h"  // Optional: for debug output

static uint32_t current_heap = 0;

// Initialize the allocator with a starting heap address
void kmalloc_init(uint32_t heap_start) {
    current_heap = heap_start;
}

// Simple bump allocator
void* kmalloc(size_t size) {
    if (current_heap == 0) {
        terminal_write("kmalloc used before initialization!\n");
        return 0;
    }

    void* addr = (void*)current_heap;
    current_heap += size;
    return addr;
}

// Allocate memory aligned to a power-of-two boundary (e.g., 0x1000)
void* kmalloc_aligned(size_t size, uint32_t alignment) {
    if (current_heap == 0) {
        terminal_write("kmalloc_aligned used before initialization!\n");
        return 0;
    }

    // Align the current_heap pointer
    if ((current_heap & (alignment - 1)) != 0) {
        current_heap = (current_heap + alignment) & ~(alignment - 1);
    }

    void* addr = (void*)current_heap;
    current_heap += size;
    return addr;
}
