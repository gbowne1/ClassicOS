#include "kmalloc.h"

static void *kernel_heap_start; // Start of the kernel heap
static void *kernel_heap_end;	// End of the kernel heap
static struct memory_block *free_blocks = NULL;

struct memory_block
{
	size_t size;
	struct memory_block *next;
};

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

void init_kernel_heap(void *start, void *end)
{
	kernel_heap_start = start;
	kernel_heap_end = end;

	free_blocks = (struct memory_block *)start;
	free_blocks->size = (size_t)(end - start);
	free_blocks->next = NULL;

	// You'll need to replace this placeholder with logic to split the
	// initial block into smaller ones based on your requirements.
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

void *kmalloc(size_t size)
{
	if (kernel_heap_start == NULL || kernel_heap_end == NULL)
	{
		// Kernel heap not initialized, cannot allocate
		return NULL;
	}

	// Align the size to the word size for efficiency
	size += sizeof(size_t) - 1;
	size &= ~(sizeof(size_t) - 1);

	// Search for a free block of sufficient size
	void *block = find_free_kernel_block(size);
	if (block != NULL)
	{
		// Mark the block as used
		mark_as_used_kernel(block, size);

		// This is a placeholder for splitting the block if necessary
		// You'll need to implement logic to potentially split the block
		// into a used part of size 'size' and a remaining free block
		// if the block size is significantly larger than 'size'.

		return block;
	}

	// No suitable block found, out of memory
	return NULL;
}

void kfree(void *ptr)
{
	if (ptr == NULL)
	{
		return;
	}

	// Mark the block as free
	// This is a placeholder for the actual algorithm
	mark_as_free_kernel(ptr);
}
