#include <stdint.h>
#include <string.h>
#include "io.h"
#include "paging.h"

page_directory_entry_t *page_directory = (page_directory_entry_t *)0x200000;
page_table_entry_t     *page_table     = (page_table_entry_t *)0x201000;

// Helper function to set up the page directory entry
void set_page_directory(page_directory_entry_t *dir) {
    // Set first PDE
    dir[0].present = 1;
    dir[0].rw = 1;
    dir[0].addr = (uint32_t)page_table >> 12;
}

// Helper function to set up the page table entry
void set_page_table(page_table_entry_t *table) {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        // Set up page table entries with identity mapping
        table[i].present = 1;
        table[i].rw = 1;        // Read/Write
        table[i].addr = i;      // Identity mapping
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
    // Zero out the tables
    memset(page_directory, 0x00, PAGE_DIRECTORY_SIZE * sizeof *page_directory);
    memset(page_table, 0x00, PAGE_TABLE_SIZE * sizeof *page_table);

    // Set up identity-mapped page directory + table
    set_page_directory(page_directory);
    set_page_table(page_table);

    enable_paging();
}
