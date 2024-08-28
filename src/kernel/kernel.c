#include "kernel.h"
#include "../kernel/malloc/kmalloc.h"
#include "../kernel/malloc/malloc.h"
#include "./arch/x86/gdt.h"
#include "./arch/x86/idt.h"
#include "./arch/x86/include/memory.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Debug levels
typedef enum
{
	DEBUG_BASIC = 1,
	DEBUG_DETAILED = 2
} debug_level_t;

// Error codes
typedef enum
{
	ERR_GDT_INIT = 1,
	ERR_IDT_INIT,
	ERR_KERNEL_HEAP_INIT,
	ERR_USER_HEAP_INIT
} error_code_t;

#define USER_HEAP_START ((uintptr_t)0x10000000)  // Example address
#define USER_HEAP_END   ((uintptr_t)0x20000000)  // Example address

// Function prototypes
static void log_message(const char *message, debug_level_t level);
bool init_gdt();

bool initialize_idt(void);
int init_kernel_heap(void *start, void *end);
int init_user_heap(void *start, void *end);
static void kernel_panic(const char *message, int error_code);
extern bool (*gdt_init_ptr)(void) = (bool (*)(void))gdt_init;

debug_level_t DEBUG_LEVEL = DEBUG_BASIC;

static void log_message(const char *message, debug_level_t level)
{
	if (level <= DEBUG_LEVEL)
	{
		printf("[LOG] %s\n", message);
	}
}

extern bool gdt_init(void)
{
	log_message("Initializing GDT...", 1);
	if (!init_gdt())
	{
		log_message("Failed to initialize GDT!", 2);
		return false;
	}
	log_message("GDT initialized successfully.", 1);
	return true;
}

bool init_gdt()
{
	// Implementation of init_gdt()
	// This is just a placeholder, you'll need to implement the actual GDT initialization logic
	return true;
}

bool initialize_idt(void)
{
    // Implement your IDT initialization logic here
    // This is just a placeholder implementation
    log_message("Initializing IDT...", DEBUG_DETAILED);

    // Your IDT initialization code goes here
    // For example:
    // - Set up IDT entries
    // - Load the IDT using lidt instruction

    // For now, let's assume it always succeeds
    return true;
}

bool InitializeIDT()
{
    log_message("Initializing IDT...", 1);
    if (!initialize_idt())
    {
        log_message("Failed to initialize IDT!", 2);
        return false;
    }
    log_message("IDT initialized successfully.", 1);
    return true;
}

int init_kernel_heap(void *start, void *end)
{
    log_message("Initializing kernel heap...", 1);
    // Implementation here
    // For now, let's assume it always succeeds
    log_message("Kernel heap initialized successfully.", 1);
    return 0;  // Return 0 for success
}

int init_user_heap(void *start, void *end)
{
    log_message("Initializing user heap...", 1);
    // Implementation here
    // For now, let's assume it always succeeds
    log_message("User heap initialized successfully.", 1);
    return 0;  // Return 0 for success
}

static void kernel_panic(const char *message, int error_code)
{
	printf("KERNEL PANIC: %s (Error code: %d)\n", message, error_code);
	// Implement any necessary error handling or system halt procedures
	while (1)
	{ /* Halt the system */
	}
}

int main()
{
	log_message("Starting kernel initialization...", 1);

	if (!gdt_init())
	{
		log_message("Initialization failed - exiting.", 2);
		return 1;
	}

	if (!InitializeIDT())
	{
		log_message("Initialization failed - exiting.", 2);
		return 1;
	}

	if (init_kernel_heap((void *)KERNEL_HEAP_START, (void *)KERNEL_HEAP_END) != 0)
	{
		log_message("Kernel heap initialization failed - exiting.", 2);
		return 1;
	}

	if (init_user_heap((void* )USER_HEAP_START, (void* )USER_HEAP_END) != 0)
	{
		log_message("User heap initialization failed - exiting.", 2);
		return 1;
	}

	log_message("Kernel initialization completed successfully.", 1);

	// Enter the main kernel loop or start the shell
	while (1)
	{
		// Kernel loop or shell code
	}

	return 0;
}
