#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

void  init_heap(void *start, size_t size);
void *kmalloc(size_t size);
void  kfree(void *ptr);

#endif /* MEMORY_H */