#include "cpu.h"
#include "serial.h"
#include "terminal.h"
#include "utils.h"
#include "print.h"

void cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile (
        "cpuid"
        : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
        : "a"(function)
    );
}

void identify_cpu() {
    uint32_t eax, ebx, ecx, edx;
    char vendor[13];

    cpuid(0, &eax, &ebx, &ecx, &edx);

    *(uint32_t *)&vendor[0] = ebx;
    *(uint32_t *)&vendor[4] = edx;
    *(uint32_t *)&vendor[8] = ecx;
    vendor[12] = '\0';

    terminal_write("CPU Vendor: ");
    terminal_write(vendor);
    terminal_write("\n");

    serial_write("CPU Vendor: ");
    serial_write(vendor);
    serial_write("\n");

    terminal_write("CPUID max leaf: ");
    print_hex(eax);  // You must implement this (see below)
    terminal_write("\n");
}
