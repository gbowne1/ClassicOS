#ifndef USB_H
#define USB_H

#include <stdint.h>
#include <stdbool.h>

// USB initialization and management functions
bool usb_init(void);
bool usb_detect_version(uint16_t *version);
bool usb_enumerate_devices(void);

// HID-specific functions
bool usb_hid_init(void);

// USB transfer functions
bool usb_control_transfer(/* parameters */);
bool usb_interrupt_transfer(/* parameters */);
bool usb_bulk_transfer(/* parameters */);

// USB host controller initialization (USB 1.x only)
bool uhci_init(void); // UHCI support
bool ohci_init(void); // OHCI support

#endif // USB_H
