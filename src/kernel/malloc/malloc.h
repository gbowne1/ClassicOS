#ifndef MALLOC_H
#define MALLOC_H

#include <stddef.h> // For size_t

// Define the memory block structure
struct memory_block {
    size_t size;
    struct memory_block *next;
    int is_free;
};

// Function prototypes
void init_heap(void *start, void *end);
void *malloc(size_t size);
void free(void *ptr);

// Helper function prototypes
void *find_free_block(size_t size);
void mark_as_used(void *ptr, size_t size);
void mark_as_free(void *ptr);

// External heap boundaries
extern void *user_heap_start;
extern void *user_heap_end;

#endif // MALLOC_H
