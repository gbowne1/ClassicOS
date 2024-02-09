#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void init_heap(void *start, size_t size);
void *kmalloc(size_t size);
void kfree(void *ptr);
void *malloc(size_t size); // Adding malloc and free declarations
void free(void *ptr);

#endif /* MEMORY_H */
