#include "pci.h"

/* --- Internal I/O Helpers (If not defined in your kernel) --- */
static inline void outl(uint16_t port, uint32_t val) {
    asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* --- Configuration Access Functions --- */

uint32_t pci_config_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((uint32_t)1 << 31) | 
                       ((uint32_t)bus << 16) | 
                       ((uint32_t)slot << 11) | 
                       ((uint32_t)func << 8) | 
                       (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void pci_config_write_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data) {
    uint32_t address = (uint32_t)((uint32_t)1 << 31) | 
                       ((uint32_t)bus << 16) | 
                       ((uint32_t)slot << 11) | 
                       ((uint32_t)func << 8) | 
                       (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    outl(PCI_CONFIG_DATA, data);
}

/* To read a word or byte, we read the Dword and shift/mask */
uint16_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t dword = pci_config_read_dword(bus, slot, func, offset);
    return (uint16_t)((dword >> ((offset & 2) * 8)) & 0xFFFF);
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t dword = pci_config_read_dword(bus, slot, func, offset);
    return (uint8_t)((dword >> ((offset & 3) * 8)) & 0xFF);
}

/* --- BAR Decoding Logic --- */

pci_bar_t pci_get_bar(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar_index) {
    pci_bar_t bar = {0};
    uint8_t offset = PCI_REG_BAR0 + (bar_index * 4);
    
    uint32_t initial_val = pci_config_read_dword(bus, slot, func, offset);
    
    // The Size Masking Trick
    pci_config_write_dword(bus, slot, func, offset, 0xFFFFFFFF);
    uint32_t mask = pci_config_read_dword(bus, slot, func, offset);
    pci_config_write_dword(bus, slot, func, offset, initial_val); // Restore
    
    if (initial_val & 0x1) {
        // I/O Space BAR
        bar.is_io = true;
        bar.base_address = initial_val & 0xFFFFFFFC;
        bar.size = ~(mask & 0xFFFFFFFC) + 1;
    } else {
        // Memory Space BAR
        bar.is_io = false;
        bar.base_address = initial_val & 0xFFFFFFF0;
        bar.is_prefetchable = (initial_val & 0x8) != 0;
        bar.size = ~(mask & 0xFFFFFFF0) + 1;
    }
    
    return bar;
}

/* --- Enumeration and Discovery --- */

void pci_check_function(uint8_t bus, uint8_t slot, uint8_t func) {
    uint16_t vendor_id = pci_config_read_word(bus, slot, func, PCI_REG_VENDOR_ID);
    if (vendor_id == 0xFFFF) return; 

    uint16_t device_id = pci_config_read_word(bus, slot, func, PCI_REG_DEVICE_ID);
    uint8_t class_code = pci_config_read_byte(bus, slot, func, PCI_REG_CLASS);
    
    /* Optional: Set Master Latency Timer if it is 0. 
       A value of 32 (0x20) or 64 (0x40) is typical. 
    */
    uint8_t latency = pci_config_read_byte(bus, slot, func, PCI_REG_LATENCY_TIMER);
    if (latency == 0) {
        // pci_config_write_byte would be needed here, or write a dword with the byte modified
        uint32_t reg_0c = pci_config_read_dword(bus, slot, func, 0x0C);
        reg_0c |= (0x20 << 8); // Set latency to 32
        pci_config_write_dword(bus, slot, func, 0x0C, reg_0c);
    }

    // Replace with your kernel's print/logging function
    // printf("Found PCI Device: %x:%x Class: %x at %d:%d:%d\n", vendor_id, device_id, class_code, bus, slot, func);
}

void pci_init(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            // Check Function 0 first
            uint16_t vendor = pci_config_read_word(bus, slot, 0, PCI_REG_VENDOR_ID);
            if (vendor == 0xFFFF) continue;

            pci_check_function(bus, slot, 0);

            // Check if this is a multi-function device
            uint8_t header_type = pci_config_read_byte(bus, slot, 0, PCI_REG_HEADER_TYPE);
            if (header_type & 0x80) {
                // Check functions 1-7
                for (uint8_t func = 1; func < 8; func++) {
                    pci_check_function(bus, slot, func);
                }
            }
        }
    }
}
