#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void init_heap(void *start, size_t size);
void *kmalloc(size_t size);
void kfree(void *ptr);
void *malloc(size_t size); // Adding malloc and free declarations
void free(void *ptr);

extern char KERNEL_HEAP_START;
extern char KERNEL_HEAP_END;

// User heap boundaries
extern char USER_HEAP_START;
extern char USER_HEAP_END;

extern char _kernel_heap_start; // Defined by the linker script

#define KERNEL_HEAP_START (&_kernel_heap_start)
#define KERNEL_HEAP_END (KERNEL_HEAP_START + 0x100000) // Adjust the size as needed

#endif /* MEMORY_H */
