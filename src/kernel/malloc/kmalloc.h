#ifndef KMALLOC_H
#define KMALLOC_H

#include <stddef.h> // For size_t

void *kmalloc(size_t size);
void kfree(void *ptr);

#endif // KMALLOC_H
