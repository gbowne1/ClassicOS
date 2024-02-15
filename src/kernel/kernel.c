#include "kernel.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_WHITE 0xF

uint16_t* const VGA_MEMORY = (uint16_t*) VGA_ADDRESS;

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = ((uint16_t)VGA_COLOR_BLACK << 8) | ' ';
    }
}

void print(const char* message) {
    static int row = 0;
    static int col = 0;
    for (size_t i = 0; message[i] != '\0'; i++) {
        if (col >= VGA_WIDTH || message[i] == '\n') {
            row++;
            col = 0;
        }
        if (row >= VGA_HEIGHT) {
            clear_screen();
            row = 0;
        }
        if (message[i] != '\n') {
            VGA_MEMORY[row * VGA_WIDTH + col] = ((uint16_t)VGA_COLOR_WHITE << 8) | message[i];
            col++;
        }
    }
}

void init_memory_management() {
    // Placeholder for actual implementation
}

void init_devices() {
    // Placeholder for actual implementation
}

void kernel_main() {
    clear_screen();
    print("Welcome to ClassicOS!");

    // Initialize memory management
    init_memory_management();

    // Initialize devices
    init_devices();

    // Infinite loop to keep the kernel running
    while (1) {
        // Handle any pending interrupts
        handle_interrupts();

        // Schedule the next process to run
        schedule_next_process();

        // Perform deferred work from interrupts or system calls
        process_deferred_work();

        // Handle system calls from user-space applications
        handle_system_calls();
    }
}

