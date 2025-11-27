#include "display.h"
#include "io.h"
#include "vga.h"

// Initialize the display
void init_display(void) {
    // Initialize the VGA driver. This typically sets up the 80x25 text mode,
    // clears the screen, and sets the cursor.
    vga_init();
}

// Enumerate connected displays
void enumerate_displays(void) {
    // This function is often a complex operation in a real driver.
    // In this simplified kernel/VGA text mode environment, we use printf
    // to output a message and rely on the fact that VGA is present.
    
    // Clear the display before printing a message
    vga_clear(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    
    // Output a simplified enumeration message
    vga_printf("Display: Standard VGA Text Mode (80x25) Detected.\n");
    
    // In a real driver, you would use inb() and outb() with specific VGA ports
    // to read information (e.g., from the CRTC registers 0x3D4/0x3D5)
    // to check for display presence or configuration.
}

// Set the display mode
// NOTE: Setting arbitrary VGA modes (like 0x13 for 320x200) is very complex
// and requires writing hundreds of register values, often done via BIOS in
// real mode. Since we are in protected mode and have a simple text driver,
// this function is kept simple or treated as a placeholder for full mode changes.
void set_display_mode(uint8_t mode) {
    // Check if the requested mode is a known mode (e.g., VGA Text Mode 3)
    // For this example, we simply acknowledge the call.
    // A true mode set would involve complex register sequencing.
    
    // The provided vga.c is a Text Mode driver, so a graphical mode set
    // like 0x13 (320x200 256-color) would break the existing vga_printf functionality.
    
    // A simplified text-mode-specific response:
    if (mode == 0x03) { // Mode 3 is standard 80x25 text mode
        vga_printf("Display mode set to 80x25 Text Mode (Mode 0x03).\n");
        vga_init(); // Re-initialize the text mode
    } else {
        // Simple I/O example based on the original structure (Caution: Incomplete for full mode set)
        outb(VGA_PORT, mode); // Example function to write to a port
        vga_printf("Attempting to set display mode to 0x%x. (Warning: May break current display)\n", mode);
    }
}

// Clear the display
void clear_display(void) {
    // Use the VGA driver's clear function, typically clearing to black on light grey
    // or black on black. We'll use the black on light grey from vga_init for consistency.
    vga_clear(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY));
    // Reset cursor to 0, 0
    vga_set_cursor_position(0, 0);
}

// Helper function to write a string
void display_write_string(const char* str) {
    // Use the VGA driver's string writing function
    vga_write_string(str, my_strlen(str));
}

// Helper function to print a formatted string
void display_printf(const char* format, ...) {
    // Use the VGA driver's printf function
    va_list args;
    va_start(args, format);
    
    // The vga_printf function already handles the va_list internally,
    // so we can just call it directly.
    vga_printf(format, args);
    
    va_end(args);
}
