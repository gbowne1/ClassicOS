#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>
#include <stddef.h> // for size_t

void kmalloc_init(uint32_t heap_start);
void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size, uint32_t alignment);

#endif
