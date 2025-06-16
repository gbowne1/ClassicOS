#ifndef MOUSE_H
#define MOUSE_H

#include <stdint.h>
#include <stdbool.h>

// Mouse data structure
typedef struct {
    int x;
    int y;
    bool left_button;
    bool right_button;
} mouse_data_t;

// Function declarations for USB 1.x HID mouse support
bool usb_mouse_init(void);
bool usb_mouse_detected(void);
bool usb_mouse_received(void);
mouse_data_t usb_read_mouse(void);

// USB Host Controller Interface functions (USB 1.x support)
bool uhci_init(void);
bool ohci_init(void);

#endif // MOUSE_H

