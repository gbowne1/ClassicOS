#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

// Specific Intel Model Definitions for your targets
#define INTEL_FAM4_486_DX      0x00 // Also 0x01
#define INTEL_FAM4_486_SX      0x02
#define INTEL_FAM4_486_DX2     0x03
#define INTEL_FAM4_486_DX4     0x08
#define INTEL_FAM5_PENTIUM     0x01 // P5
#define INTEL_FAM5_PENTIUM_MMX 0x04 // P55C
#define INTEL_FAM6_PENTIUM_PRO 0x01 // P6
#define INTEL_FAM6_PENTIUM_II  0x05 // Deschutes
#define INTEL_FAM6_PENTIUM_III 0x07 // Katmai/Coppermine
#define INTEL_FAM15_P4_WILLY   0x00 // Willamette
#define INTEL_FAM15_P4_NORTH   0x02 // Northwood
#define INTEL_FAM15_P4_PRES    0x03 // Prescott

typedef struct {
    char vendor[13];
    uint32_t family;
    uint32_t model;
    uint32_t stepping;
    uint32_t type;
    uint32_t max_leaf;
    
    // Feature flags (optional, but very helpful later)
    bool has_fpu;
    bool has_mmx;
    bool has_sse;
} cpu_info_t;

// Function Prototypes
void cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);
bool cpu_check_cpuid_support(void);
void identify_cpu(void);

// Helper to get the current CPU info after identification
cpu_info_t* cpu_get_info(void);

#endif // CPU_H
