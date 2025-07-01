#include "malloc.h"
#include <stdint.h>

static void *heap_start; // Start of the heap
static void *heap_end;   // End of the heap
static struct memory_block *free_blocks; // List of free blocks

void init_heap(void *start, void *end)
{
    heap_start = start;
    heap_end = end;

    // Initialize the heap with a single large free block
    free_blocks = (struct memory_block *)start;
    free_blocks->size = (uintptr_t)end - (uintptr_t)start - sizeof(struct memory_block);
    free_blocks->next = NULL;
    free_blocks->is_free = 1;
}

void *find_free_block(size_t size) {
    struct memory_block *current = free_blocks;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    // No suitable block found
    return NULL;
}

void mark_as_used(void *ptr, size_t size) {
    struct memory_block *block = (struct memory_block *)ptr;
    block->is_free = 0;

    // If the block is larger than needed, split it
    if (block->size > size + sizeof(struct memory_block)) {
        struct memory_block *new_block = (struct memory_block *)((uintptr_t)ptr + size + sizeof(struct memory_block));
        new_block->size = block->size - size - sizeof(struct memory_block);
        new_block->next = block->next;
        new_block->is_free = 1;

        block->size = size;
        block->next = new_block;
    }
}

void mark_as_free(void *ptr) {
    struct memory_block *block = (struct memory_block *)ptr;
    block->is_free = 1;

    // Coalesce with next block if it's free
    if (block->next && block->next->is_free) {
        block->size += block->next->size + sizeof(struct memory_block);
        block->next = block->next->next;
    }

    // Coalesce with previous block if it's free
    struct memory_block *prev = free_blocks;
    while (prev && prev->next != block) {
        prev = prev->next;
    }
    if (prev && prev->is_free) {
        prev->size += block->size + sizeof(struct memory_block);
        prev->next = block->next;
    }
}

void *malloc(size_t size)
{
    if (heap_start == NULL || heap_end == NULL)
    {
        // Heap not initialized, cannot allocate
        return NULL;
    }

    // Align the size to the word size for efficiency
    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);

    // Search for a free block of sufficient size
    void *block = find_free_block(size);
    if (block != NULL)
    {
        // Mark the block as used
        mark_as_used(block, size);
        return (void *)((uintptr_t)block + sizeof(struct memory_block));
    }

    // No suitable block found, out of memory
    return NULL;
}

void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }

    // Adjust pointer to the start of the memory block
    struct memory_block *block = (struct memory_block *)((uintptr_t)ptr - sizeof(struct memory_block));

    // Mark the block as free
    mark_as_free(block);
}
