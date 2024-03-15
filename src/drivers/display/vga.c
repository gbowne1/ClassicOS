#include "vga.h"
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t *map_memory(uint32_t base_address, uint32_t size);

#define VGA_MEMORY_ADDRESS 0xA0000
#define VGA_MEMORY_SIZE 0x60000
#define VGA_MEMORY_BASE_ADDRESS 0xA0000

#define COLOR_GREEN 0x00FF00

uint8_t *vga_memory = (uint8_t *)VGA_MEMORY_ADDRESS;
static uint16_t vga_width = VGA_WIDTH;
static uint16_t vga_height = VGA_HEIGHT;
static uint8_t vga_depth = VGA_DEPTH;
uint8_t *framebuffer = (uint8_t *)vga_memory;

// Set a specific pixel color (assuming RGB format)
int x = 10;
int y = 20;
int color = 0x00FF00; // Green

framebuffer[y * width + x * 3] = (color & 0x00FF) >> 8; // Green value
framebuffer[y * width + x * 3 + 1] = color & 0x00FF;	// Blue value
framebuffer[y * width + x * 3 + 2] = color >> 8;		// Red value

uint8_t misc_output_value = read_vga_register(0x3C2);

// Check bit 1 to determine text mode (0) or graphics mode (1)
if (misc_output_value & 0x02)
{
	printf("Graphics mode active\n");
}
else
{
	printf("Text mode active\n");
}

// Function to retrieve the actual VGA base memory address (if needed)
uint32_t get_vga_memory_address()
{
	// Implement logic to query the system for the actual VGA base address
	// This might involve consulting BIOS information, system calls, etc.
	// Return the retrieved address if successful, otherwise return the default value.
	return VGA_MEMORY_ADDRESS;
}

void vga_init(void)
{
	// Initialize VGA driver here
	vga_memory = (uint8_t *)map_memory(VGA_MEMORY_BASE_ADDRESS, VGA_MEMORY_SIZE);

	if (vga_memory == NULL)
	{
		// Handle memory mapping error
		fprintf(stderr, "Failed to map VGA Memory\n");
	}

	framebuffer = vga_memory;
}

// Replace with actual functions to read VGA registers based on your system
uint8_t read_vga_register(uint16_t register_address)
{
    uint8_t value = inb(register_address);  // Read from the specified port
    return value;
}

int inb(uint16_t port) {
    // Implement port access logic (replace with system-specific function)
}

void outb(uint16_t port, uint8_t data) {

}

void vga_set_resolution(uint16_t width, uint16_t height, uint8_t depth)
{
	// Set VGA resolution here
	// Example (replace with actual register values and writes based on your system):

	// Sequencer unlock
	outb(0x3C4, 0x01);
	outb(0x3C5, 0x01);

	// Set specific CRTC registers for desired resolution and refresh rate
	// ... (write to specific VGA registers using outb)

	// Graphics controller registers for memory access
	// ... (write to specific VGA registers using outb)
}

void vga_draw_pixel(uint16_t x, uint16_t y, uint8_t color)
{
	// Draw a pixel on the screen at (x, y) with the given color
	uint32_t offset = y * vga_width + x;
	vga_memory[offset] = color;
}

void vga_clear_screen(uint8_t color)
{
	// Clear the screen with the given color
	for (uint32_t i = 0; i < vga_width * vga_height; i++)
	{
		vga_memory[i] = color;
	}
}
