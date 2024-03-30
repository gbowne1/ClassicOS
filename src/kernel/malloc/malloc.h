#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h> // For size_t

void *malloc(size_t size);
void free(void *ptr);

void *find_free_block(size_t size);

#endif // MALLOC_H
