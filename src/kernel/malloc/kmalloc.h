#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>
#include <stddef.h>

// Structure to represent a memory block in the kernel heap
typedef struct {
  void* base;  // Starting address of the block
  size_t size;  // Size of the block in bytes
  struct heap_block* next;  // Pointer to the next block in the free list
} heap_block_t;

// Function prototypes
void init_kernel_heap(void* start, void* end);
void init_user_heap(void* start, void* end);
void* kmalloc(size_t size);
void kfree(void* ptr);

// Global variable to store the free list head
extern heap_block_t* kheap_free_list;

#endif
 /* KMALLOC_H_ */
