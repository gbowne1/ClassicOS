#include <stddef.h>
#include "vesa.h"
#include "io.h"
#include "print.h"

// VESA mode and controller information
#define VESA_BIOS_INT 0x10
#define VESA_BIOS_FUNC 0x4F

// Function to call BIOS with specific VESA function
static bool vesa_bios_call(uint16_t function, uint16_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
    // Set up registers for VESA function call
    __asm__ __volatile__(
        "movw %1, %%ax\n"           // Move function number into AX
        "int $0x10\n"                // Call BIOS interrupt 0x10 (VESA)
        "movw %%ax, %0\n"            // Move return value in AX to the return variable
        : "=m"(*eax)                 // Output operand (eax)
        : "m"(function)              // Input operand (function number)
        : "%eax", "%ebx", "%ecx", "%edx", "memory"
    );
    // Check for success (return values vary depending on the function)
    return *eax == 0x004F;
}

// Set the VESA video mode
bool vesa_set_mode(uint16_t mode) {
    uint16_t eax = VBE_FUNCTION_SET_MODE;
    uint32_t ebx = mode;
    uint32_t ecx = 0;
    uint32_t edx = 0;
    
    if (vesa_bios_call(VBE_FUNCTION_SET_MODE, &eax, &ebx, &ecx, &edx)) {
        return true;
    }
    return false;
}

// Get the VESA mode information
bool vesa_get_mode_info(uint16_t mode, vbe_mode_info_t* info) {
    uint16_t ax_ret;
    
    // Convert the 32-bit pointer to a Segment:Offset pair
    // IMPORTANT: 'info' MUST be located in the first 1MB of RAM
    uint32_t ptr = (uint32_t)info;
    uint16_t segment = (uint16_t)((ptr >> 4) & 0xFFFF);
    uint16_t offset  = (uint16_t)(ptr & 0x000F);

    __asm__ __volatile__(
        "push %%es\n\t"         // Save Protected Mode ES
        "movw %1, %%es\n\t"     // Load the segment into ES
        "int $0x10\n\t"         // BIOS Interrupt
        "pop %%es\n\t"          // Restore Protected Mode ES
        : "=a"(ax_ret)          // Result in AX
        : "r"(segment),         // %1
          "D"(offset),          // %2 (DI)
          "a"((uint16_t)VBE_FUNCTION_GET_MODE_INFO), // AX (0x4F01)
          "c"(mode)             // CX (The Mode Number)
        : "memory", "cc"        // Removed %es from clobbers
    );

    return ax_ret == 0x004F;
}

bool vesa_get_controller_info(vbe_controller_info_t* info) {
    uint16_t ax_ret;
    
    // We must use Segment:Offset for BIOS
    uint32_t ptr = (uint32_t)info;
    uint16_t segment = (uint16_t)((ptr >> 4) & 0xF000); // Base segment
    uint16_t offset  = (uint16_t)(ptr & 0xFFFF);        // Offset

    // Copy "VBE2" into signature to tell BIOS we want VBE 2.0+ info
    info->Signature[0] = 'V';
    info->Signature[1] = 'B';
    info->Signature[2] = 'E';
    info->Signature[3] = '2';

    // To fix the error: Do not use %es in clobber. 
    // Use a temporary register or a push/pop sequence if we MUST touch ES.
    __asm__ __volatile__(
        "push %%es\n\t"
        "movw %1, %%es\n\t"
        "int $0x10\n\t"
        "pop %%es\n\t"
        : "=a"(ax_ret)
        : "r"(segment), "D"(offset), "a"(0x4F00)
        : "memory", "cc" 
    );

    return ax_ret == 0x004F;
}

// Return pointer to the VESA framebuffer
void* vesa_get_framebuffer(void) {
    vbe_mode_info_t mode_info;
    if (vesa_get_mode_info(0x101, &mode_info)) {
        return (void*)mode_info.PhysBasePtr;
    }
    return NULL;
}

// Clear the screen with a color
void vesa_clear_screen(uint32_t color) {
    uint32_t* framebuffer = (uint32_t*)vesa_get_framebuffer();
    if (framebuffer) {
        for (int y = 0; y < 480; y++) { // For 640x480 mode
            for (int x = 0; x < 640; x++) {
                framebuffer[y * 640 + x] = color;
            }
        }
    }
}
