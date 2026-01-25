// mouse.c
#include "mouse.h"
#include "usb.h"
#include <stdint.h>
#include <stdbool.h>

// Mouse buffer
mouse_data_t mouse_data;

// Read USB mouse data
mouse_data_t usb_read_mouse(void) {
    uint8_t buffer[3]; // USB HID Mouse reports typically use 3 bytes
    if (usb_interrupt_transfer()) { // Ensure buffer is filled
        // Process the received data
        mouse_data.x += buffer[1]; // X movement
        mouse_data.y += buffer[2]; // Y movement
        mouse_data.left_button = buffer[0] & 0x01;
        mouse_data.right_button = buffer[0] & 0x02;
    } else {
        // Handle the case where no data was received
        // You can choose to reset mouse_data or leave it unchanged
        // For example, you might want to set movement to zero if no data is received
        // mouse_data.x = 0;
        // mouse_data.y = 0;
    }
    return mouse_data;
}
