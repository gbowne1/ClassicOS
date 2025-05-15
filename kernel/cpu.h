#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
void identify_cpu(void);

#endif // CPU_H
