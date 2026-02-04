#ifndef VESA_H
#define VESA_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// VESA BIOS Extension 2.0 Function Calls
#define VBE_FUNCTION_SET_MODE        0x4F02
#define VBE_FUNCTION_GET_MODE_INFO   0x4F01
#define VBE_FUNCTION_GET_CONTROLLER_INFO 0x4F00
#define VBE_FUNCTION_SET_DISPLAY_START 0x4F05

// VESA Mode Information Structure (VBE 2.0)
typedef struct {
    uint16_t ModeAttributes;      // Mode attributes
    uint8_t WinAAttributes;       // Window A attributes
    uint8_t WinBAttributes;       // Window B attributes
    uint16_t WinGranularity;      // Window granularity
    uint16_t WinSize;             // Window size
    uint16_t WinASegment;         // Window A segment address
    uint16_t WinBSegment;         // Window B segment address
    uint32_t WinFuncPtr;          // Function pointer for window
    uint16_t BytesPerScanLine;    // Bytes per scanline
    uint16_t XResolution;         // Horizontal resolution in pixels
    uint16_t YResolution;         // Vertical resolution in pixels
    uint8_t XCharSize;            // Character cell width
    uint8_t YCharSize;            // Character cell height
    uint8_t NumberOfPlanes;       // Number of memory planes
    uint8_t BitsPerPixel;         // Bits per pixel
    uint8_t NumberOfBanks;        // Number of banks
    uint8_t MemoryModel;          // Memory model type
    uint8_t BankSize;             // Bank size in kB
    uint8_t NumberOfImagePages;   // Number of image pages
    uint8_t Reserved0;            // Reserved
    uint8_t RedMaskSize;          // Red mask size
    uint8_t RedFieldPosition;     // Red field position
    uint8_t GreenMaskSize;        // Green mask size
    uint8_t GreenFieldPosition;   // Green field position
    uint8_t BlueMaskSize;         // Blue mask size
    uint8_t BlueFieldPosition;    // Blue field position
    uint8_t RsvdMaskSize;         // Reserved mask size
    uint8_t RsvdFieldPosition;    // Reserved field position
    uint8_t DirectColorModeInfo;  // Direct color mode info
    uint32_t PhysBasePtr;         // Physical base address of the linear framebuffer
    uint32_t OffScreenMemOff;     // Offset to off-screen memory
    uint16_t OffScreenMemSize;    // Size of off-screen memory
    uint8_t Reserved1[206];       // Reserved
} __attribute__((packed)) vbe_mode_info_t;

// VESA Controller Information

typedef struct {
    char Signature[4];             // Should be "VESA" (or "VBE2" for request)
    uint16_t Version;              // VBE version; high byte is major, low is minor
    uint32_t OEMStringPtr;         // Segment:Offset pointer to OEM string
    uint32_t Capabilities;         // Capabilities of graphics controller
    uint32_t VideoModePtr;         // Segment:Offset pointer to supported modes list
    uint16_t TotalMemory;          // Number of 64KB memory blocks
    uint16_t OEMSoftwareRev;       // VBE implementation Software revision
    uint32_t OEMVendorNamePtr;     // Segment:Offset pointer to Vendor Name string
    uint32_t OEMProductNamePtr;    // Segment:Offset pointer to Product Name string
    uint32_t OEMProductRevPtr;     // Segment:Offset pointer to Product Revision string
    uint8_t Reserved[222];         // Reserved for VBE implementation scratch area
    uint8_t OEMData[256];          // Data area for OEM strings
} __attribute__((packed)) vbe_controller_info_t;

// Function Prototypes
bool vesa_set_mode(uint16_t mode);
bool vesa_get_mode_info(uint16_t mode, vbe_mode_info_t* info);
bool vesa_get_controller_info(vbe_controller_info_t* info);
void* vesa_get_framebuffer(void);
void vesa_clear_screen(uint32_t color);

#endif // VESA_H
