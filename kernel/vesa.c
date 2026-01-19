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
    uint16_t eax = VBE_FUNCTION_GET_MODE_INFO;
    uint32_t ebx = mode;
    uint32_t ecx = 0;
    uint32_t edx = 0;

    if (vesa_bios_call(VBE_FUNCTION_GET_MODE_INFO, &eax, &ebx, &ecx, &edx)) {
        // Copy the information into the provided struct
        uint32_t base = (uint32_t)info;
        __asm__ __volatile__(
            "movw %%bx, %%es:%%di\n"
            : 
            : "b" (base)
            : "%es", "%di", "memory"
        );
        return true;
    }
    return false;
}

// Get the VESA controller information
bool vesa_get_controller_info(vbe_controller_info_t* info) {
    uint16_t eax = VBE_FUNCTION_GET_CONTROLLER_INFO;
    uint32_t ebx = 0;
    uint32_t ecx = 0;
    uint32_t edx = 0;

    if (vesa_bios_call(VBE_FUNCTION_GET_CONTROLLER_INFO, &eax, &ebx, &ecx, &edx)) {
        // Copy controller information into the provided struct
        uint32_t base = (uint32_t)info;
        __asm__ __volatile__(
            "movw %%bx, %%es:%%di\n"
            : 
            : "b" (base)
            : "%es", "%di", "memory"
        );
        return true;
    }
    return false;
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
