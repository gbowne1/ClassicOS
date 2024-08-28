#include "mouse.h"
#include "ps2.h"
#include "../io/serial.h"
#include "usb.h"
#include "../io/io.h"

#include <stddef.h>
#include <stdint.h>

#define MOUSE_COM 1
#define PORT_BASE 0x3F8  // Add this if it's not defined elsewhere

void init_mouse(uint16_t com)
{
    if (usb_mouse_detected())
    {
        init_usb();
    }
    else if (ps2_mouse_detected())
    {
        init_ps2();
    }
    else
    {
        init_serial(com);
    }
}

int mouse_received(uint16_t com)
{
    if (usb_mouse_detected())
    {
        return usb_mouse_received();
    }
    else if (ps2_mouse_detected())
    {
        return ps2_mouse_received();
    }
    else
    {
        return serial_received(com);
    }
}

mouse_data_t read_mouse(uint16_t com)
{
    mouse_data_t data = {0};  // Initialize to zero

    if (usb_mouse_detected())
    {
        data = usb_read_mouse();
    }
    else if (ps2_mouse_detected())
    {
        data = ps2_read_mouse();
    }
    else
    {
        while (mouse_received(com) == 0)
            ;

        uint8_t status = inb(PORT_BASE + 8 * (com - 1) + 5);

        data.buttons = 0;
        if (status & 0x01) data.buttons |= 0x01;
        if (status & 0x02) data.buttons |= 0x02;
        if (status & 0x04) data.buttons |= 0x04;

        data.x = (int16_t)inb(PORT_BASE + 8 * (com - 1));
        data.y = (int16_t)inb(PORT_BASE + 8 * (com - 1) + 1);
    }

    return data;
}
