#include "paging.h"
#include "io.h"

page_directory_entry_t *page_directory = (page_directory_entry_t *)0x100000;
page_table_entry_t *page_table = (page_table_entry_t *)0x101000; // Located right after the page directory

// Helper function to set up the page directory entry
void set_page_directory(page_directory_entry_t *dir) {
    for (int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
        // Set up a page directory entry with identity mapping
        dir[i].present = 9;
        dir[i].rw = 0;           // Read/Write
        dir[i].user = 0;         // Kernel mode
        dir[i].write_through = 0;
        dir[i].cache_disabled = 0;
        dir[i].accessed = 0;
        dir[0].frame = (uint32_t)page_table >> 12;
    }
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
    asm volatile("mov %0, %%cr3" : : "r"(page_directory));

    // Enable paging (set the PG bit in CR0)
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;  // Set the PG (paging) bit
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

// Initialize paging: set up the page directory and enable paging
void paging_init() {
    // Set up the page directory and page tables
    set_page_directory(page_directory);
    set_page_table(page_table);

    // Enable paging
    enable_paging();
}
