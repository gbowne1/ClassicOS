#include "cpu.h"
#include <stdint.h>
#include <stdio.h>

// Existing functions
uint32_t read_register(uint8_t reg) { /*...*/ }
void write_register(uint8_t reg, uint32_t value) { /*...*/ }
void switch_to_protected_mode() { /*...*/ }

// Integrated cpuid function
/*void cpuid(uint32_t function, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
	asm volatile(
		"cpuid"
		: "=a"(*eax),
		  "=b"(*ebx),
		  "=c"(*ecx),
		  "=d"(*edx)
		: "a"(function));
}
*/

// Optionally, integrate identify_cpu function
void identify_cpu()
{
	uint32_t max_leaf;
	uint32_t vendor_id[4];
	uint32_t eax, ebx, ecx, edx;

	cpuid(0, &eax, &ebx, &ecx, &edx);
	max_leaf = eax;

	cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
	vendor_id[0] = eax;
	vendor_id[1] = ebx;
	vendor_id[2] = ecx;
	vendor_id[3] = edx;

	printf("Vendor ID: %.4s%.4s\n", (char *)&vendor_id[0], (char *)&vendor_id[1]);
	printf("Maximum leaf value: %u\n", max_leaf);
}

int main() {
    identify_cpu();
    return 0;
}
