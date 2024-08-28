#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Existing declarations
uint32_t read_register(uint8_t reg);
void write_register(uint8_t reg, uint32_t value);
void switch_to_protected_mode();

// New declaration for cpuid
void cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

#endif // CPU_H
