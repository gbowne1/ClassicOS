#ifndef PARALLEL_H
#define PARALLEL_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LPT_PORT_NONE = -1,
    LPT1_PORT = 0,
    LPT2_PORT = 1,
    LPT_MAX_PORTS = 2
} lpt_port_t;

typedef enum {
    LPT_MODE_COMPAT = 0,   // Standard (SPP)
    LPT_MODE_BIDIR,        // PS/2 bidirectional
    LPT_MODE_EPP,          // IEEE 1284 EPP
    LPT_MODE_ECP           // IEEE 1284 ECP
} lpt_mode_t;

typedef struct {
    uint16_t base;         // Base I/O address (e.g., 0x378, 0x278)
    bool     present;      // Detected
    lpt_mode_t mode;       // Current mode
    uint8_t  irq;          // IRQ line (if known/used)
} lpt_device_t;

extern lpt_device_t lpt_devices[LPT_MAX_PORTS];

void lpt_init_all(void);
void lpt_set_mode(lpt_port_t port, lpt_mode_t mode);

// Simple polled I/O
void lpt_write_byte(lpt_port_t port, uint8_t value);
uint8_t lpt_read_byte(lpt_port_t port);

// IRQ-driven hook (you implement the handler logic)
void lpt_irq_handler(lpt_port_t port);

#endif
