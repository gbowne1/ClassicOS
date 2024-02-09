#include "../include/memory.h"
#include <stddef.h>
#include <string.h>

/* The start of the kernel heap */
static uint8_t *heap_start = NULL;

/* The end of the kernel heap */
static uint8_t *heap_end = NULL;

/* The current position of the heap */
static uint8_t *heap_pos = NULL;

/* The size of the heap */
static size_t heap_size = 0;

void init_heap(void *start, size_t size)
{
	heap_start = (uint8_t *)start;
	heap_end = heap_start + size;
	heap_pos = heap_start;
	heap_size = size;
}

void *kmalloc(size_t size)
{
	/* Round up the size to a multiple of 4 bytes */
	size = (size + 3) & ~3;

	/* Check if there is enough space in the heap */
	if (heap_pos + size > heap_end)
	{
		return NULL;
	}

	/* Allocate memory from the heap */
	void *ptr = heap_pos;
	heap_pos += size;

	return ptr;
}

// Implementing a simple malloc-like function
void *malloc(size_t size)
{
	// Round up the size to a multiple of  4 bytes
	size = (size + 3) & ~3;

	// Call kmalloc to allocate memory from the heap
	return kmalloc(size);
}

// Implementing a simple free-like function
void free(void *ptr)
{
	// Check if the pointer is within the heap
	if ((uint8_t *)ptr >= heap_start && ptr < (void *)heap_end)
	{
		// Call kfree to deallocate the memory
		kfree(ptr);
	}
}

void kfree(void *ptr)
{
	/* Check if the pointer is within the heap */
	if ((uint8_t *)ptr >= heap_start && ptr < (void *)heap_end)
	{
		/* Zero out the memory */
		volatile uint8_t *volatile_ptr = (volatile uint8_t *)ptr;
		size_t size = (uint8_t *)heap_pos - (uint8_t *)ptr;
		while (size--)
		{
			*volatile_ptr++ = 0;
		}

		/* Free the memory by moving the heap position */
		heap_pos = (uint8_t *)ptr;
	}
}
