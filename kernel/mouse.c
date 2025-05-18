#include "mouse.h"
#include "usb.h"
#include <stdint.h>
#include <stdbool.h>

// Struct to store mouse state
typedef struct {
    int x;
    int y;
    bool left_button;
    bool right_button;
} mouse_data_t;

// Mouse buffer
static mouse_data_t mouse_data;

// Initialize USB Mouse (1.x & HID 1.0-1.11)
bool usb_mouse_init(void) {
    if (!usb_init()) {
        return false;
    }

    if (!usb_hid_init()) {
        return false;
    }

    if (!usb_enumerate_devices()) {
        return false;
    }

    return true;
}

// Detect if a USB mouse is connected
bool usb_mouse_detected(void) {
    return usb_hid_init();
}

// Check if USB mouse data has been received
bool usb_mouse_received(void) {
    return usb_interrupt_transfer(/* parameters */);
}

// Read USB mouse data
mouse_data_t usb_read_mouse(void) {
    uint8_t buffer[3]; // USB HID Mouse reports typically use 3 bytes
    if (usb_interrupt_transfer(/* parameters */)) {
        // Process the received data
        mouse_data.x += buffer[1]; // X movement
        mouse_data.y += buffer[2]; // Y movement
        mouse_data.left_button = buffer[0] & 0x01;
        mouse_data.right_button = buffer[0] & 0x02;
    }
    return mouse_data;
}

// USB Host Controller Initialization (for USB 1.x)
bool uhci_init(void) {
    // Initialize UHCI controller for legacy USB support
    return true;
}

bool ohci_init(void) {
    // Initialize OHCI controller for USB 1.x
    return true;
}
