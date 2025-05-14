#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "serial.h"
#include "terminal.h"
#include "idt.h"
#include "paging.h"
#include "memmap.h"
#include "gdt.h"

#define LPT1 0x378

void lpt_write(char c) {
    while ((inb(LPT1 + 1) & 0x80) == 0); // Wait for ready
    outb(LPT1, c);
}

void kmain(void) {
    terminal_initialize();
    terminal_write("Welcome to ClassicOS\n");

    serial_init();
    serial_write("Serial port initialized.\n");

    lpt_write('L'); // Send 'L' to LPT1 to test

    terminal_write("Initializing GDT...\n");
    gdt_init();
    serial_write("GDT initialized.\n");

    terminal_write("Initializing IDT...\n");
    idt_init();
    serial_write("IDT initialized.\n");

    terminal_write("Enabling paging...\n");
    paging_init();
    serial_write("Paging initialized.\n");

    terminal_write("Initializing memory allocator...\n");
    kmalloc_init(0xC0100000);  // Virtual heap start address (must be mapped!)
    serial_write("kmalloc initialized.\n");

    void* ptr = kmalloc(128);  // Allocation test
    serial_write("Allocated 128 bytes.\n");

    terminal_write("Initializing timer...\n");
    timer_init(100);  // 100 Hz (10 ms interval)
    serial_write("Timer initialized.\n");

    terminal_write("Getting memory map...\n");
    memory_map_entry_t mmap[32];
    uint32_t mmap_size = get_memory_map(mmap, 32);
    serial_write("Memory map retrieved.\n");

    terminal_write("Memory Regions:\n");
    for (uint32_t i = 0; i < mmap_size; i++) {
        terminal_write(" - Region: ");
        // You would format and print base/length/type here
        // (e.g., with a basic itoa and print_hex helper)
        serial_write("Memory region entry\n");
    }

    terminal_write("System initialized. Halting.\n");

    // Halt CPU in loop
    while (1) {
        asm volatile("hlt");
    }
}
