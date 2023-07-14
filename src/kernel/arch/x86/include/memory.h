#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void *kmalloc(size_t size);
void  kfree(void *ptr);

#endif /* MEMORY_H */