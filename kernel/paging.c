#include "paging.h"
#include "io.h"
#include <stdint.h>
#include <stddef.h>

page_directory_entry_t *page_directory = (page_directory_entry_t *)0x100000;
page_table_entry_t *page_table = (page_table_entry_t *)0x101000;
page_table_entry_t *heap_page_table = (page_table_entry_t *)0x102000; // Located right after the page directory

// Helper function to set up the page directory entry
void set_page_directory(page_directory_entry_t *dir) {
    for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
        dir[i].present = 0;
    }
    dir[0].present = 1;
    dir[0].rw = 1;
    dir[0].user = 0;
    dir[0].frame = (uint32_t)page_table >> 12;
}

// Helper function to set up the page table entry
void set_page_table(page_table_entry_t *table) {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        // Set up page table entries with identity mapping
        table[i].present = 1;
        table[i].rw = 1;          // Read/Write
        table[i].user = 0;        // Kernel mode
        table[i].write_through = 0;
        table[i].cache_disabled = 0;
        table[i].accessed = 0;
        table[i].frame = i;       // Identity mapping
    }
}

// Enable paging by loading the page directory into CR3 and setting the PG bit in CR0
void enable_paging() {
    uint32_t cr0;

    // Load page directory into CR3
    __asm__("mov %0, %%cr3" : : "r"(page_directory));

    // Enable paging (set the PG bit in CR0)
    __asm__("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set the PG (paging) bit
    __asm__("mov %0, %%cr0" : : "r"(cr0));
}

// Initialize paging: set up the page directory and enable paging
void paging_init() {
    // Set up identity-mapped page directory + table
    set_page_directory(page_directory);
    set_page_table(page_table);

    // === Set up heap mapping at 0xC0100000 ===
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        heap_page_table[i].present = 1;
        heap_page_table[i].rw = 1;
        heap_page_table[i].user = 0;
        heap_page_table[i].write_through = 0;
        heap_page_table[i].cache_disabled = 0;
        heap_page_table[i].accessed = 0;
        heap_page_table[i].frame = (256 + i); // Start physical heap at 1MB (256*4KB = 1MB)
    }

    // Index 772 = 0xC0100000 / 4MB
    page_directory[772].present = 1;
    page_directory[772].rw = 1;
    page_directory[772].user = 0;
    page_directory[772].frame = (uint32_t)heap_page_table >> 12;

    enable_paging();
}
