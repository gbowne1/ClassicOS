#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

// ACPI base address (replace with actual value)
#define ACPI_BASE 0xE0000000

#ifndef NULL
#define NULL ((void*)0)
#endif

// ACPI RSDP Structure (Root System Description Pointer)
typedef struct {
    uint8_t signature[8];      // Should be "RSD PTR "
    uint8_t checksum;          // Checksum for the RSDP structure
    uint8_t oem_id[6];         // OEM ID string
    uint8_t revision;          // ACPI revision
    uint32_t rsdt_addr;        // 32-bit RSDT address (ACPI 1.0)
    uint32_t xsdt_addr;        // 64-bit XSDT address (ACPI 2.0+)
} __attribute__((packed)) acpi_rsdp_t;

// ACPI RSDT Structure (Root System Description Table)
typedef struct {
    uint32_t signature;    // Should be "RSDT"
    uint32_t length;       // Length of the table
    uint8_t revision;      // Revision of the RSDT table
    uint8_t checksum;      // Checksum for the RSDT table
    uint32_t tables[];     // Array of pointers to other tables (RSDT/XSDT entries)
} __attribute__((packed)) acpi_rsdt_t;

// ACPI FADT Structure (Fixed ACPI Description Table)
typedef struct {
    uint32_t signature;         // Should be "FACP"
    uint32_t length;            // Length of the table
    uint8_t revision;           // Revision of the FADT table
    uint8_t checksum;           // Checksum for the FADT table
    uint32_t pm_tmr_address;    // Power Management Timer Address
    // ... other FADT fields
} __attribute__((packed)) acpi_fadt_t;

// Function prototypes
acpi_rsdp_t* acpi_find_rsdp();
void* acpi_get_rsdt_or_xsdt(acpi_rsdp_t* rsdp);
acpi_fadt_t* acpi_find_fadt(void* rsdt_or_xsdt);

#endif /* ACPI_H */
