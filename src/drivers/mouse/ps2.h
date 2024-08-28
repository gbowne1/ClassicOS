#ifndef PS2_H
#define PS2_H

#include <stdint.h>
#include <stdbool.h>

// PS/2 Controller Ports
#define PS2_DATA_PORT    0x60
#define PS2_STATUS_PORT  0x64
#define PS2_COMMAND_PORT 0x64

// PS/2 Controller Commands
#define PS2_CMD_READ_CONFIG      0x20
#define PS2_CMD_WRITE_CONFIG     0x60
#define PS2_CMD_DISABLE_PORT2    0xA7
#define PS2_CMD_ENABLE_PORT2     0xA8
#define PS2_CMD_TEST_PORT2       0xA9
#define PS2_CMD_TEST_CONTROLLER  0xAA
#define PS2_CMD_TEST_PORT1       0xAB
#define PS2_CMD_DISABLE_PORT1    0xAD
#define PS2_CMD_ENABLE_PORT1     0xAE

// Function prototypes
bool ps2_init(void);
uint8_t ps2_read_data(void);
void ps2_write_data(uint8_t data);
uint8_t ps2_read_status(void);
void ps2_write_command(uint8_t command);
bool ps2_wait_for_read(void);
bool ps2_wait_for_write(void);

// PS/2 device specific functions
bool ps2_keyboard_init(void);
bool ps2_mouse_init(void);

#endif // PS2_H
