#ifndef PS2_H
#define PS2_H

#include <stdint.h>
#include <stdbool.h>

/* I/O Ports */
#define PS2_DATA_PORT       0x60
#define PS2_STATUS_REG      0x64
#define PS2_COMMAND_REG     0x64

/* Status Register Bits */
#define PS2_STATUS_OUTPUT   0x01 // 1 = Data ready to be read
#define PS2_STATUS_INPUT    0x02 // 1 = Controller busy, don't write yet
#define PS2_STATUS_SYS      0x04 // System flag
#define PS2_STATUS_CMD_DATA 0x08 // 0 = Data written to 0x60, 1 = Cmd to 0x64
#define PS2_STATUS_MOUSE    0x20 // 1 = Mouse data, 0 = Keyboard data

/* Controller Commands */
#define PS2_CMD_READ_CONFIG  0x20
#define PS2_CMD_WRITE_CONFIG 0x60
#define PS2_CMD_DISABLE_MS   0xA7
#define PS2_CMD_ENABLE_MS    0xA8
#define PS2_CMD_DISABLE_KB   0xAD
#define PS2_CMD_ENABLE_KB    0xAE
#define PS2_CMD_WRITE_MOUSE  0xD4

/* Mouse Commands */
#define MOUSE_CMD_SET_DEFAULTS 0xF6
#define MOUSE_CMD_ENABLE_SCAN  0xF4

typedef struct {
    int8_t  x_delta;
    int8_t  y_delta;
    bool    left_button;
    bool    right_button;
    bool    middle_button;
} mouse_state_t;

/* Public API */
void ps2_init(void);
void ps2_keyboard_handler(void);
void ps2_mouse_handler(void);

#endif
