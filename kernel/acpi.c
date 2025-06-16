#include "acpi.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Function to find the RSDP (Root System Description Pointer)
acpi_rsdp_t* acpi_find_rsdp() {
    // Search memory from 0x000E0000 to 0x00100000 (BIOS)
    for (uint32_t addr = 0x000E0000; addr < 0x00100000; addr += 16) {
        acpi_rsdp_t* rsdp = (acpi_rsdp_t*)addr;
        if (memcmp(rsdp->signature, "RSD PTR ", 8) == 0) {
            uint8_t checksum = 0;
            for (size_t i = 0; i < sizeof(acpi_rsdp_t); i++) { // Change int to size_t
                checksum += ((uint8_t*)rsdp)[i];
            }
            if (checksum == 0) {
                return rsdp;  // Valid RSDP found
            }
        }
    }
    return NULL;  // RSDP not found
}

// Function to get the RSDT or XSDT based on the RSDP revision
void* acpi_get_rsdt_or_xsdt(acpi_rsdp_t* rsdp) {
    if (rsdp->revision >= 2) {
        return (void*)rsdp->xsdt_addr;  // ACPI 2.0+ uses XSDT
    } else {
        return (void*)rsdp->rsdt_addr;  // ACPI 1.0 uses RSDT
    }
}

// Function to find the FADT table within the RSDT or XSDT
acpi_fadt_t* acpi_find_fadt(void* rsdt_or_xsdt) {
    acpi_rsdt_t* rsdt = (acpi_rsdt_t*)rsdt_or_xsdt;
    uint32_t num_tables = (rsdt->length - sizeof(acpi_rsdt_t)) / sizeof(uint32_t);

    for (size_t i = 0; i < num_tables; i++) {
        uint32_t table_addr = rsdt->tables[i];
        acpi_fadt_t* fadt = (acpi_fadt_t*)table_addr;
        if (fadt->signature == 0x50434146) {
            uint8_t checksum = 0;
            for (size_t j = 0; j < fadt->length; j++) {
                checksum += ((uint8_t*)fadt)[j];
            }
            if (checksum == 0) {
                return fadt;
            }
        }
    }
    return NULL;
}

