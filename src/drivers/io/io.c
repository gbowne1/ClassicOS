#include "io.h"

/*
    Common Ports 
    COM1: 0x3F8
    COM2: 0x2F8
    COM3: 0x3E8
    COM4: 0x2E8
    LPT1: 0x378
    LPT2: 0x278
    LPT3: 0x3BC
*/

// Function to initialize the COM and LPT ports
void io_init()
{
    // TODO: Initialize the COM and LPT ports
    // Set up any necessary configuration or control operations
}

// Function to read from the COM port
char io_read_com()
{
    // TODO: Read from the COM port
    // Use the appropriate memory or I/O address to read from the port
    // Return the read data
}

// Function to write to the COM port
void io_write_com(char data)
{
    // TODO: Write to the COM port
    // Use the appropriate memory or I/O address to write to the port
    // Write the provided data to the port
}

// Function to read from the LPT port
char io_read_lpt()
{
    // TODO: Read from the LPT port
    // Use the appropriate memory or I/O address to read from the port
    // Return the read data
}

// Function to write to the LPT port
void io_write_lpt(char data)
{
    // TODO: Write to the LPT port
    // Use the appropriate memory or I/O address to write to the port
    // Write the provided data to the port
}