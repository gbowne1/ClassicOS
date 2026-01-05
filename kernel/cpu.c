#include "cpu.h"
#include "serial.h"
#include "terminal.h"
#include "utils.h"

// Helper to print a labeled decimal value
void print_val(const char* label, uint32_t val) {
    char buf[12];
    utoa(val, buf, 10);
    terminal_write(label);
    terminal_write(buf);
    terminal_write(" ");
}

// Safely check if CPUID is supported by attempting to flip bit 21 of EFLAGS
int check_cpuid_supported() {
    uint32_t f1, f2;
    __asm__ volatile (
        "pushfl\n\t"
        "pushfl\n\t"
        "popl %0\n\t"
        "movl %0, %1\n\t"
        "xorl $0x200000, %0\n\t"
        "pushl %0\n\t"
        "popfl\n\t"
        "pushfl\n\t"
        "popl %0\n\t"
        "popfl\n\t"
        : "=&r" (f1), "=&r" (f2));
    return ((f1 ^ f2) & 0x200000) != 0;
}

void identify_cpu() {
    if (!check_cpuid_supported()) {
        terminal_write("CPUID not supported. Likely a 386 or early 486.\n");
        return;
    }

    uint32_t eax, ebx, ecx, edx;
    char vendor[13];

    // Leaf 0: Vendor String & Max Leaf
    cpuid(0, &eax, &ebx, &ecx, &edx);
    uint32_t max_leaf = eax;
    *(uint32_t *)&vendor[0] = ebx;
    *(uint32_t *)&vendor[4] = edx;
    *(uint32_t *)&vendor[8] = ecx;
    vendor[12] = '\0';

    terminal_write("Vendor: ");
    terminal_write(vendor);
    terminal_write("\n");

    // Leaf 1: Family, Model, Stepping
    if (max_leaf >= 1) {
        cpuid(1, &eax, &ebx, &ecx, &edx);
        
        uint32_t stepping = eax & 0xF;
        uint32_t model = (eax >> 4) & 0xF;
        uint32_t family = (eax >> 8) & 0xF;
        uint32_t type = (eax >> 12) & 0x3;

        // Handle Extended Family/Model (Required for Pentium 4 and newer)
        if (family == 0xF) {
            family += (eax >> 20) & 0xFF;
            model += ((eax >> 16) & 0xF) << 4;
        }

        print_val("Family:", family);
        print_val("Model:", model);
        print_val("Step:", stepping);
        terminal_write("\n");
    }

    // Leaf 2: Cache Descriptors
    if (max_leaf >= 2) {
        cpuid(2, &eax, &ebx, &ecx, &edx);
        
        terminal_write("Cache Descriptors: ");
        // Note: Leaf 2 returns a list of 1-byte descriptors in the registers.
        // We look for common Intel ones:
        uint32_t regs[4] = {eax, ebx, ecx, edx};
        for (int i = 0; i < 4; i++) {
            if (regs[i] & 0x80000000) continue; // Reserved bit
            for (int j = 0; j < 4; j++) {
                uint8_t desc = (regs[i] >> (j * 8)) & 0xFF;
                if (desc == 0) continue;
                
                // Example decoding for specific chips you mentioned:
                if (desc == 0x06) terminal_write("8KB L1 I-Cache ");
                if (desc == 0x0A) terminal_write("8KB L1 D-Cache ");
                if (desc == 0x41) terminal_write("128KB L2 ");
                if (desc == 0x43) terminal_write("512KB L2 ");
                if (desc == 0x2C) terminal_write("32KB L1 D-Cache ");
            }
        }
        terminal_write("\n");
    }
}
