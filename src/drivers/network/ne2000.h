#ifndef NE2000_H
#define NE2000_H

#include <stdint.h>

// NE2000 registers
#define NE2000_COMMAND 0x00
#define NE2000_PSTART 0x01
#define NE2000_PSTOP 0x02
// ... more registers ...

// NE2000 commands
#define NE2000_CMD_START 0x02
#define NE2000_CMD_STOP 0x01
// ... more commands ...

// Function prototypes
void ne2000_write_reg(uint16_t base_addr, uint8_t reg, uint8_t value);
uint8_t ne2000_read_reg(uint16_t base_addr, uint8_t reg);
void ne2000_init(uint16_t base_addr);
// ... more function prototypes ...

#endif // NE2000_H