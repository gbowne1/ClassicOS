#ifndef IO_H
#define IO_H

// Function to initialize the COM and LPT ports
void io_init();

// Function to read from the COM port
char io_read_com();

// Function to write to the COM port
void io_write_com(char data);

// Function to read from the LPT port
char io_read_lpt();

// Function to write to the LPT port
void io_write_lpt(char data);

#endif /* IO_H */