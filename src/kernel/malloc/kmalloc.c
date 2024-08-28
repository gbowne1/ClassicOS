#include "kmalloc.h"
#include <stdbool.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HEAP_MAGIC 0x123456789ABCDEF0

static void *kernel_heap_start; // Start of the kernel heap
static void *kernel_heap_end;	// End of the kernel heap
static struct memory_block *free_blocks = NULL;

struct memory_block
{
	size_t size;
	struct memory_block *next;
};

typedef struct
{
	size_t size;
	bool is_free;
	uint64_t magic; // For integrity checking
} block_meta_t;

static void *heap_start;
static void *heap_end;
static block_meta_t *free_list;

void mark_as_used_kernel(void *ptr, size_t size)
{
	// Since we're using a linked list, there's no specific action needed
	//  to mark the block itself as used (it's already removed from the list).
	// However, you might want to perform additional bookkeeping for debugging
	// or memory analysis purposes.

	// Example: Update some metadata associated with the allocated block
	// ((struct memory_block *)ptr)->used = true; // Assuming a 'used' flag

	// This function can be potentially empty if you don't require specific
	// actions for marking a block as used in the kernel.
}

void mark_as_free_kernel(void *ptr)
{
	// Cast the pointer to a memory_block structure
	struct memory_block *block = (struct memory_block *)ptr;

	// Add the block to the beginning of the free block list
	block->next = free_blocks;
	free_blocks = block;
}

int init_kernel_heap(void *start, void *end)
{
	if (start >= end)
	{
		return -1; // Invalid range
	}

	heap_start = start;
	heap_end = end;
	size_t total_size = (char *)end - (char *)start;

	if (total_size < sizeof(block_meta_t))
	{
		return -2; // Heap too small
	}

	// Initialize the first block
	block_meta_t *first_block = (block_meta_t *)start;
	first_block->size = total_size - sizeof(block_meta_t);
	first_block->is_free = true;
	first_block->magic = HEAP_MAGIC;

	// Initialize the free list with the first block
	free_list = first_block;

	return 0; // Success
}

void *kmalloc(size_t size)
{
    block_meta_t *current = free_list;

    while (current) {
        if (current->magic != HEAP_MAGIC) {
            // Heap corruption detected
            return NULL;
        }

        if (current->is_free && current->size >= size) {
            // Found a suitable block
            if (current->size > size + sizeof(block_meta_t)) {
                // Split the block
                block_meta_t *new_block = (block_meta_t*)((char*)current + sizeof(block_meta_t) + size);
                new_block->size = current->size - size - sizeof(block_meta_t);
                new_block->is_free = true;
                new_block->magic = HEAP_MAGIC;

                current->size = size;
            }

            current->is_free = false;
            return (void*)((char*)current + sizeof(block_meta_t));
        }

        current = (block_meta_t*)((char*)current + sizeof(block_meta_t) + current->size);

        if ((void*)current >= heap_end) {
            break;  // Reached end of heap
        }
    }

    // No suitable block found
    return NULL;
}

void kfree(void *ptr)
{
	if (!ptr)
	{
		return;
	}

	block_meta_t *block_meta = (block_meta_t *)((char *)ptr - sizeof(block_meta_t));

	if (block_meta->magic != HEAP_MAGIC)
	{
		// Invalid free or heap corruption
		return;
	}

	block_meta->is_free = true;

	// Coalesce with next block if it's free
	block_meta_t *next_block = (block_meta_t *)((char *)block_meta + sizeof(block_meta_t) + block_meta->size);
	if ((void *)next_block < heap_end && next_block->is_free)
	{
		block_meta->size += sizeof(block_meta_t) + next_block->size;
	}

	// TODO: Implement coalescing with previous block
}

void *find_free_kernel_block(size_t size)
{
	// First-fit algorithm (can be optimized with different strategies)
	struct memory_block *current = free_blocks;
	while (current != NULL)
	{
		if (current->size >= size)
		{
			// Found a suitable block
			return current;
		}
		current = current->next;
	}
	// No suitable block found
	return NULL;
}
