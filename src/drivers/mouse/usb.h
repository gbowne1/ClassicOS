#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <stdbool.h>
#include "./mouse.h"

// USB Version definitions
#define USB_VERSION_1_0 0x100
#define USB_VERSION_1_1 0x110
#define USB_VERSION_2_0 0x200
#define USB_VERSION_2_1 0x210

// USB device classes
#define USB_CLASS_HID 0x03

// HID subclasses
#define USB_SUBCLASS_BOOT 0x01

// HID protocols
#define USB_PROTOCOL_KEYBOARD 0x01
#define USB_PROTOCOL_MOUSE    0x02

// Function prototypes
mouse_data_t usb_read_mouse(void);
bool usb_mouse_detected(void);
bool usb_mouse_received(void);
bool usb_init(void);
bool usb_detect_version(uint16_t *version);
bool usb_enumerate_devices(void);

// HID device specific functions
bool usb_hid_init(void);
bool usb_keyboard_init(void);
bool usb_mouse_init(void);

// USB transfer functions
bool usb_control_transfer(/* parameters */);
bool usb_interrupt_transfer(/* parameters */);
bool usb_bulk_transfer(/* parameters */);

// USB host controller interface functions
bool uhci_init(void);  // USB 1.x
bool ohci_init(void);  // USB 1.x
bool ehci_init(void);  // USB 2.0
bool xhci_init(void);  // USB 3.0+ (for future use)

#endif // USB_H
