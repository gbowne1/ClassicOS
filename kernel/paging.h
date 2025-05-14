#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096                // Page size in bytes
#define PAGE_DIRECTORY_SIZE 1024      // 1024 entries in page directory
#define PAGE_TABLE_SIZE 1024          // 1024 entries in a page table
#define KERNEL_HEAP_START 0xC0100000

// Page Directory and Page Table structure
typedef struct {
    uint32_t present : 1;  // Present bit (1: page is present in memory)
    uint32_t rw : 1;       // Read-Write bit (1: page is read-write)
    uint32_t user : 1;     // User-supervisor bit (1: user mode access)
    uint32_t write_through : 1;  // Write-through cache
    uint32_t cache_disabled : 1; // Cache disabled
    uint32_t accessed : 1;        // Accessed bit
    uint32_t reserved : 1;        // Reserved bit
    uint32_t page_size : 1;       // Page size (0: 4KB, 1: 4MB)
    uint32_t global : 1;           // Global page (can be used across different processes)
    uint32_t available : 3;        // Available bits for the system
    uint32_t frame : 20;           // Frame address (physical address)
} __attribute__((packed)) page_table_entry_t;

// Define page directory entry
typedef struct {
    uint32_t present : 1;
    uint32_t rw : 1;
    uint32_t user : 1;
    uint32_t write_through : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 1;
    uint32_t zero : 5;           // Must be zero for page directory
    uint32_t reserved_2 : 7;     // Reserved bits
    uint32_t frame : 20;         // Frame address of the page table
} __attribute__((packed)) page_directory_entry_t;

extern page_directory_entry_t *page_directory;
extern page_table_entry_t *page_table;
extern page_table_entry_t *heap_page_table;

void paging_init(void);
void set_page_directory(page_directory_entry_t *dir);
void set_page_table(page_table_entry_t *table);
void enable_paging(void);

#endif
