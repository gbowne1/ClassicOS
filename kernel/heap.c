#include "heap.h"
#include <stdint.h>

typedef struct heap_block {
    size_t size;
    struct heap_block *next;
    int is_free;
} heap_block_t;

static heap_block_t *free_list = NULL;
static void *heap_start_ptr = NULL;
static void *heap_end_ptr = NULL;

void heap_init(void *heap_start, void *heap_end) {
    heap_start_ptr = heap_start;
    heap_end_ptr = heap_end;

    free_list = (heap_block_t *)heap_start;
    free_list->size = (uintptr_t)heap_end - (uintptr_t)heap_start - sizeof(heap_block_t);
    free_list->next = NULL;
    free_list->is_free = 1;
}

void *heap_alloc(size_t size) {
    heap_block_t *current = free_list;

    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            // If it's a large block, split it
            if (current->size > size + sizeof(heap_block_t)) {
                heap_block_t *new_block = (heap_block_t *)((uintptr_t)current + sizeof(heap_block_t) + size);
                new_block->size = current->size - size - sizeof(heap_block_t);
                new_block->next = current->next;
                new_block->is_free = 1;

                current->next = new_block;
                current->size = size;
            }

            current->is_free = 0;
            return (void *)((uintptr_t)current + sizeof(heap_block_t));
        }

        current = current->next;
    }

    return NULL; // Out of memory
}

void heap_free(void *ptr) {
    if (ptr == NULL) return;

    heap_block_t *block = (heap_block_t *)((uintptr_t)ptr - sizeof(heap_block_t));
    block->is_free = 1;

    // Coalesce with next block
    if (block->next && block->next->is_free) {
        block->size += block->next->size + sizeof(heap_block_t);
        block->next = block->next->next;
    }

    // TODO: Coalesce with previous block for better compaction
}
