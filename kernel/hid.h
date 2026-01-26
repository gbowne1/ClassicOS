#ifndef HID_H
#define HID_H

#include <stdint.h>
#include <stdbool.h>

// HID Report types
#define HID_REPORT_INPUT  0x01
#define HID_REPORT_OUTPUT 0x02
#define HID_REPORT_FEATURE 0x03

// HID usage page constants (USB HID)
#define HID_USAGE_PAGE_GENERIC 0x01
#define HID_USAGE_KEYBOARD 0x06
#define HID_USAGE_MOUSE 0x02

// HID keyboard and mouse data
typedef struct {
    uint8_t modifier;    // Modifier keys (shift, ctrl, alt, etc.)
    uint8_t reserved;    // Reserved byte
    uint8_t keycodes[6]; // Keycodes for keys pressed
} keyboard_hid_report_t;

typedef struct {
    uint8_t buttons;     // Mouse buttons (bitwise: 0x01 = left, 0x02 = right, 0x04 = middle)
    int8_t x;            // X axis movement
    int8_t y;            // Y axis movement
    int8_t wheel;        // Mouse wheel
} mouse_hid_report_t;

// Initialize the HID subsystem
void hid_init(void);

// Process an incoming HID report
void hid_process_report(uint8_t* report, uint8_t length);

// Process HID keyboard report
void hid_process_keyboard_report(const keyboard_hid_report_t* report);

// Process HID mouse report
void hid_process_mouse_report(const mouse_hid_report_t* report);

// USB HID report descriptor parsing
bool hid_parse_descriptor(uint8_t* descriptor, uint32_t length);

#endif // HID_H
