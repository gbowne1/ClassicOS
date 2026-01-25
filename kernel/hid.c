#include "hid.h"
#include "usb.h"
#include "mouse.h"
#include "keyboard.h"
#include "print.h"
#include <stdint.h>
#include <stdbool.h>

// Global variables
static bool hid_initialized = false;

void hid_init(void) {
    if (hid_initialized) return;
    hid_initialized = true;

    // Initialize keyboard and mouse HID handling
    keyboard_init();
    // Assume USB mouse has been initialized and is connected.
    usb_hid_init(); // Initializes USB HID for both keyboard and mouse
}

void hid_process_report(uint8_t* report, uint8_t length) {
    // Process the HID report based on its type
    if (length == 8) {  // Assuming a standard 8-byte report for HID keyboard
        keyboard_hid_report_t* k_report = (keyboard_hid_report_t*) report;
        hid_process_keyboard_report(k_report);
    } else if (length == 3) {  // Assuming a standard 3-byte report for HID mouse
        mouse_hid_report_t* m_report = (mouse_hid_report_t*) report;
        hid_process_mouse_report(m_report);
    }
}

// Handle HID keyboard report
void hid_process_keyboard_report(const keyboard_hid_report_t* report) {
    // Iterate over the keycodes and process key presses
    for (int i = 0; i < 6; i++) {
        uint8_t keycode = report->keycodes[i];
        if (keycode != 0) {
            char key = scancode_map[keycode];
            if (key) {
                keyboard_buffer_add(key);
            }
        }
    }
}

// Handle HID mouse report
void hid_process_mouse_report(const mouse_hid_report_t* report) {
    // Process mouse movement and button clicks
    mouse_data.x += report->x;
    mouse_data.y += report->y;
    mouse_data.left_button = (report->buttons & 0x01) != 0;
    mouse_data.right_button = (report->buttons & 0x02) != 0;

    print_hex((uint32_t)mouse_data.x, 1, 1);
    print_hex((uint32_t)mouse_data.y, 1, 1);
    print_hex((uint32_t)report->buttons, 1, 1);
}

// Parse the HID descriptor (for parsing USB HID device descriptors)
bool hid_parse_descriptor(uint8_t* descriptor, uint32_t length) {
    // HID descriptors are defined in the USB HID specification, we'll need to parse them here.
    // For now, just return true assuming we have a valid descriptor.
    return true;
}
