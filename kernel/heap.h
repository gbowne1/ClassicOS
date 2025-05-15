#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

void heap_init(void *heap_start, void *heap_end);

void *heap_alloc(size_t size);
void heap_free(void *ptr);

#endif // HEAP_H
