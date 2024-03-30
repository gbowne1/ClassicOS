#include "vga.h"
#include <memory.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t *map_memory(uint32_t base_address, uint32_t size);
uint8_t *vga_memory = (uint8_t *)VGA_MEMORY_ADDRESS;
static uint16_t vga_width = VGA_WIDTH;
static uint16_t vga_height = VGA_HEIGHT;
static uint8_t vga_depth = VGA_DEPTH;
uint8_t *framebuffer = (uint8_t *)vga_memory;
uint8_t *map_memory(uint32_t base_address, uint32_t size);
uint8_t read_vga_register(uint16_t register_address);
int inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint16_t width;
uint32_t get_vga_memory_address();

// Set a specific pixel color (assuming RGB format)
int x = 10;
int y = 20;
int color = 0x00FF00; // Green

void some_function()
{
	// ... other code ...
	uint8_t misc_output_value = read_vga_register(0x3C2); // Initialize here
														  // ... use misc_output_value ...
}

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
	vga_memory = (uint8_t *)map_memory(VGA_MEMORY_ADDRESS, VGA_MEMORY_SIZE);

	if (vga_memory == NULL)
	{
		// Handle memory mapping error
		fprintf(stderr, "Failed to map VGA Memory\n");
	}

	framebuffer = (uint8_t *)vga_memory;
}

// Replace with actual functions to read VGA registers based on your system
uint8_t read_vga_register(uint16_t register_address)
{
	uint8_t value = inb(register_address); // Read from the specified port
	return value;
}

int inb(uint16_t port)
{
	uint8_t value;
	__asm volatile("inb %1, %0" : "=a"(value) : "d"(port));
	return value;
}

void outb(uint16_t port, uint8_t data)
{
	__asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

void vga_set_resolution(uint16_t width, uint16_t height, uint8_t depth)
{

	// Sequencer unlock
	outb(0x3C4, 0x01);
	outb(0x3C5, 0x01);

	// Set specific CRTC registers for desired resolution and refresh rate
	outb(0x3D4, 0x01); // Horizontal total
	outb(0x3D5, 0x5F); // ...
	// ... (set other CRTC registers)

	// Graphics controller registers for memory access
	outb(0x3CE, 0x05); // Graphics mode
	outb(0x3CF, 0x03);
}

void vga_draw_pixel(uint16_t x, uint16_t y, uint8_t color)
{
	// Draw a pixel on the screen at (x, y) with the given color
	uint32_t offset = y * vga_width + x;
	vga_memory[offset] = color;
	framebuffer[y * width + x * 3] = (color & 0x00FF) >> 8; // Green value
	framebuffer[y * width + x * 3 + 1] = color & 0x00FF;	// Blue value
	framebuffer[y * width + x * 3 + 2] = color >> 8;		// Red value
}

void vga_clear_screen(uint8_t color)
{
	// Clear the screen with the given color
	for (uint32_t i = 0; i < vga_width * vga_height; i++)
	{
		vga_memory[i] = color;
	}
}
