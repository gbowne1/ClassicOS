#include "malloc.h"

static void *heap_start; // Start of the heap
static void *heap_end;	 // End of the heap

void init_heap(void *start, void *end)
{
	heap_start = start;
	heap_end = end;
	// Initialize the heap here
}

void *find_free_block(size_t size) {
  // Implementation is similar to find_free_kernel_block
  struct memory_block *current = free_blocks;
  while (current != NULL) {
    if (current->size >= size) {
      return current;
    }
    current = current->next;
  }

  // No suitable block found
  return NULL;
}

void *malloc(size_t size)
{
	if (heap_start == NULL || heap_end == NULL)
	{
		// Heap not initialized, cannot allocate
		return NULL;
	}

	// Align the size to the word size for efficiency
	size &= ~(sizeof(size_t) - 1);

	// Search for a free block of sufficient size
	// This is a placeholder for the actual algorithm
	void *block = find_free_block(size);
	if (block != NULL)
	{
		// Mark the block as used
		mark_as_used(block, size);
		return block;
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

	// Mark the block as free
	// This is a placeholder for the actual algorithm
	mark_as_free(ptr);
}
