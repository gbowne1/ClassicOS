#include "io.h"

/*
    Common Ports
    COM1: 0x3F8
    COM2: 0x2F8
    COM3: 0x3E8
    COM4: 0x2E8
    COM5: 0x5F8
    COM6: 0x4F8
    COM7: 0x5E8
    COM8: 0x4E8
    LPT1: 0x378
    LPT2: 0x278
    LPT3: 0x3BC
*/

// Function to initialize the ports before reading or writing
void io_init()
{
    // Initialize COM1 port (0x3F8) - You can add more port initializations here if needed

    // Example initialization for COM1 port (9600 baud, 8N1)
    outb(0x3F8 + 1, 0x00);   // Disable all interrupts
    outb(0x3F8 + 3, 0x80);   // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);   // Set divisor to 3 (lo byte) for 9600 baud rate
    outb(0x3F8 + 1, 0x00);   // Set divisor to hi byte for 9600 baud rate (default)
    outb(0x3F8 + 3, 0x03);   // Set data format to 8N1 (8 data bits, no parity, one stop bit)

}

// Function to read from the COM port
char io_read_com()
{
    // Read from COM1 port 0x3F8
    return inb(0x3F8); // Read data from COM1 port
}

// Function to write to the COM port
void io_write_com(char data)
{
    // Write to COM1 port 0x3F8
    outb(0x3F8, data); // Write data to COM1 port
}

// Function to read from the LPT port
char io_read_lpt()
{
    // Read from LPT1 port 0x378
    return inb(0x378); // Read data from LPT1 port
}

// Function to write to the LPT port
void io_write_lpt(char data)
{
    // Write to LPT1 port 0x378
    outb(0x378, data); // Write data to LPT1 port
}
