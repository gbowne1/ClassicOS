#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

// ACPI base address (replace with actual value)
#define ACPI_BASE 0xE0000000

// ACPI Fixed ACPI Description Table (FADT) structure
typedef struct
{
	uint32_t signature;		 // Signature ("RSDT" or "XSDT")
	uint32_t length;		 // Length of the table
	uint8_t revision;		 // ACPI revision
	uint8_t checksum;		 // Checksum (should be zero)
	uint32_t oem_id[6];		 // OEM ID string
	uint8_t oem_revision[8]; // OEM revision string
	uint32_t rsdp_addr;		 // Physical address of RSDT (for ACPI 1.0)
	uint32_t xsdt_addr;		 // Physical address of XSDT (for ACPI 2.0+)
							 // ... other FADT fields (refer to ACPI specification)
} __attribute__((packed)) acpi_fadt_t;

// ... (Add other ACPI table structure definitions as needed)

#endif /* ACPI_H */
