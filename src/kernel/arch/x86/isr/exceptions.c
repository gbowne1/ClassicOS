#include "exceptions.h"

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PAGE_SIZE 4096
typedef struct memory_region_t
{
	void *start_address;
	size_t size;
} memory_region_t;

// Array of allocated memory regions (example)
memory_region_t *allocated_regions;
size_t num_allocated_regions;

jmp_buf page_fault_buffer;

void DivideByZero()
{
	// Add logic to handle Divide By Zero Exception
	printf("Divide By Zero Exception\n");

	// Additional Exception Handling Logic:
	// Example: Perform specific actions for Divide By Zero scenario
	// - Log the exception to a file
	FILE *logFile = fopen("error.log", "a");
	if (logFile != NULL)
	{
		fprintf(logFile, "Divide By Zero Exception occurred\n");
		fclose(logFile);
	}

	// - Gracefully terminate the kernel
	printf("Exiting kernel due to Divide By Zero Exception\n");
	exit(EXIT_FAILURE);
}

void DoubleFault()
{
	printf("Double Fault Exception\n");
	// Handle double fault exception (typically unrecoverable)
	exit(EXIT_FAILURE);
}

// Function to check if address is within allocated memory (example)
int IsAddressInRange(uint16_t address)
{
	for (size_t i = 0; i < num_allocated_regions; ++i)
	{
		if ((address >= allocated_regions[i].start_address) &&
			(address < (allocated_regions[i].start_address + allocated_regions[i].size)))
		{
			return 1;
		}
	}
	return 0;
}

// Placeholder for loading page from disk (replace with actual implementation)
void LoadPageFromDisk(uint16_t fault_address, void *page_buffer)
{
	// Implement logic to read page data from disk based on fault_address
	// and store it in the provided page_buffer
	printf("** Placeholder: Load page from disk (implementation needed) **\n");
}

// Placeholder for modifying paging tables (replace with actual implementation)
void NotifyCPUAboutPage(uint16_t fault_address)
{
	// Implement logic to modify paging tables based on fault_address
	// to point to the loaded page data
	printf("** Placeholder: Modify paging tables (implementation needed) **\n");
}

void PageFault(uint16_t fault_address)
{
	// 1. Identify the cause of the page fault
	if (IsAddressInRange(fault_address))
	{
		// Address is within allocated memory, potentially a missing page
		printf("Page fault for address 0x%x (potentially missing page)\n", fault_address);

		// 2. Allocate memory for the page data (assuming OS doesn't handle this)
		void *page_buffer = malloc(PAGE_SIZE); // Replace with actual page size
		if (page_buffer == NULL)
		{
			printf("Failed to allocate memory for page data\n");
			exit(EXIT_FAILURE);
		}

		// 3. Load the missing page from disk
		LoadPageFromDisk(fault_address, page_buffer);

		// 4. Inform the CPU that the page is now available
		NotifyCPUAboutPage(fault_address);

		// 5. Free the temporary page buffer (if applicable)
		free(page_buffer);

		// 6. Resume execution with longjmp
		longjmp(page_fault_buffer, 1);
	}
	else
	{
		// Handle invalid memory access
		printf("Invalid memory access at 0x%x\n", fault_address);
		exit(EXIT_FAILURE); // Or perform alternative error handling
	}
}

void GeneralProtectionFault()
{
	printf("General Protection Fault Exception\n");
	// Handle general protection fault exception (access violation)
	exit(EXIT_FAILURE);
}
