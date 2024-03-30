#ifndef KMALLOC_H_  // Corrected guard macro
#define KMALLOC_H_

#include <stddef.h> // For size_t

void *kmalloc(size_t size);
void kfree(void *ptr);

void mark_as_used_kernel(void *ptr, size_t size);
void mark_as_free_kernel(void *ptr);

#endif /* KMALLOC_H_ */
