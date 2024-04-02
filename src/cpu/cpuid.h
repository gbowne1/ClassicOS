#ifndef CPUID_H
#define CPUID_H

// Function prototypes for CPUID instruction
void cpuid(uint32_t code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

#endif
