#include "cpuid.h"
#include <stdint.h>

void cpuid(uint32_t code, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
  asm volatile ("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (code));
}

void identify_cpu() {
  uint32_t max_leaf;
  uint32_t vendor_id[4];
  uint32_t eax, ebx, ecx, edx;

  // Get the maximum supported leaf value (CPUID function)
  cpuid(0, &eax, &ebx, &ecx, &edx);
  max_leaf = eax;

  // Get the vendor ID string
  cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
  vendor_id[0] = eax;
  vendor_id[1] = ebx;
  vendor_id[2] = ecx;
  vendor_id[3] = edx;

  // Print the vendor ID string (assuming ASCII characters)
  printf("Vendor ID: %.4s%.4s\n", (char *)&vendor_id[0], (char *)&vendor_id[1]);

  // Identify basic features based on CPUID information (optional, needs further logic)
  // ... (code to check specific CPU features using max_leaf and additional CPUID calls) ...

  printf("Maximum leaf value: %u\n", max_leaf);
}

int main() {
  identify_cpu();
  return 0;
}
