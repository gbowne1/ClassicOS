#include "parallel.h"
#include "io.h"
#include "irq.h"
#include "serial.h"   // or your print/terminal for debug

// Standard PC LPT base addresses
static const uint16_t lpt_base_addrs[LPT_MAX_PORTS] = {
    0x378, // LPT1
    0x278  // LPT2
};

lpt_device_t lpt_devices[LPT_MAX_PORTS];

// Register offsets
#define LPT_DATA(base)      (base + 0)
#define LPT_STATUS(base)    (base + 1)
#define LPT_CONTROL(base)   (base + 2)

// STATUS bits
// bit 7: Busy (inverted), 6: Ack, 5: Paper Out, 4: Select, 3: Error
// CONTROL bits
// bit 0: Strobe, 1: Auto Linefeed, 2: Init, 3: Select In, 5: Bidirectional (PS/2)

// Simple presence check: write/read control & status
static bool lpt_detect(uint16_t base) {
    uint8_t orig_ctrl = inb(LPT_CONTROL(base));
    outb(LPT_CONTROL(base), orig_ctrl ^ 0x0F);
    uint8_t new_ctrl = inb(LPT_CONTROL(base));
    outb(LPT_CONTROL(base), orig_ctrl);

    // If bits changed as expected, port likely exists
    if (((orig_ctrl ^ new_ctrl) & 0x0F) == 0x0F) {
        return true;
    }
    return false;
}

static void lpt_configure_bidir(uint16_t base, bool enable) {
    uint8_t ctrl = inb(LPT_CONTROL(base));
    if (enable) {
        ctrl |= (1 << 5);   // Set bidirectional bit (PS/2)
    } else {
        ctrl &= ~(1 << 5);
    }
    outb(LPT_CONTROL(base), ctrl);
}

void lpt_set_mode(lpt_port_t port, lpt_mode_t mode) {
    if (port < 0 || port >= LPT_MAX_PORTS) return;
    if (!lpt_devices[port].present) return;

    uint16_t base = lpt_devices[port].base;

    switch (mode) {
        case LPT_MODE_COMPAT:
            lpt_configure_bidir(base, false);
            break;
        case LPT_MODE_BIDIR:
            lpt_configure_bidir(base, true);
            break;
        case LPT_MODE_EPP:
            // TODO: EPP requires chipset support & config
            // For now, just enable bidir as a baseline
            lpt_configure_bidir(base, true);
            break;
        case LPT_MODE_ECP:
            // TODO: ECP requires FIFO, DMA, and ECR register
            // Stub for future implementation
            lpt_configure_bidir(base, true);
            break;
    }

    lpt_devices[port].mode = mode;
}

void lpt_write_byte(lpt_port_t port, uint8_t value) {
    if (port < 0 || port >= LPT_MAX_PORTS) return;
    if (!lpt_devices[port].present) return;

    uint16_t base = lpt_devices[port].base;

    // Wait until not busy (bit 7 is inverted busy)
    while (!(inb(LPT_STATUS(base)) & 0x80))
        ;

    outb(LPT_DATA(base), value);

    // Pulse strobe
    uint8_t ctrl = inb(LPT_CONTROL(base));
    outb(LPT_CONTROL(base), ctrl | 0x01);
    outb(LPT_CONTROL(base), ctrl & ~0x01);
}

uint8_t lpt_read_byte(lpt_port_t port) {
    if (port < 0 || port >= LPT_MAX_PORTS) return 0xFF;
    if (!lpt_devices[port].present) return 0xFF;

    uint16_t base = lpt_devices[port].base;

    // In bidirectional mode, data register is input
    return inb(LPT_DATA(base));
}

// IRQ hook: you wire this into your IRQ handler for the LPT IRQ (usually 7 or 5)
void lpt_irq_handler(lpt_port_t port) {
    // For now, just a stub. Later:
    // - read status
    // - acknowledge interrupt
    // - wake waiting writer/reader
    (void)port;
}

// Initialize all LPT ports
void lpt_init_all(void) {
    for (int i = 0; i < LPT_MAX_PORTS; i++) {
        lpt_devices[i].base = lpt_base_addrs[i];
        lpt_devices[i].present = lpt_detect(lpt_devices[i].base);
        lpt_devices[i].mode = LPT_MODE_COMPAT;
        lpt_devices[i].irq = 0; // You can fill this if you parse BIOS/PCI/ACPI

        if (lpt_devices[i].present) {
            serial_write("LPT detected at base 0x");
            // use your print_hex here if you want
        }
    }

    // If you want interrupt-driven I/O:
    // - Map LPT IRQ (usually 7 for LPT1, 5 for LPT2) in your PIC/IRQ layer
    // - In your IRQ handler, call lpt_irq_handler(port)
}
