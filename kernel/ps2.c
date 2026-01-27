#include "ps2.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %b0, %w1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %w1, %b0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* --- Controller Synchronization --- */

// Wait until the controller is ready to receive a byte
static void ps2_wait_write() {
    while (inb(PS2_STATUS_REG) & PS2_STATUS_INPUT);
}

// Wait until the controller has a byte for us to read
static void ps2_wait_read() {
    while (!(inb(PS2_STATUS_REG) & PS2_STATUS_OUTPUT));
}

/* --- Initialization --- */

void ps2_write_device(uint8_t command) {
    ps2_wait_write();
    outb(PS2_DATA_PORT, command);
}

void ps2_write_mouse(uint8_t data) {
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_WRITE_MOUSE); // "Next byte goes to mouse"
    ps2_wait_write();
    outb(PS2_DATA_PORT, data);
}

void ps2_init(void) {
    // 1. Disable Devices
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_DISABLE_KB);
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_DISABLE_MS);

    // 2. Flush Output Buffer
    while (inb(PS2_STATUS_REG) & PS2_STATUS_OUTPUT) {
        inb(PS2_DATA_PORT);
    }

    // 3. Set Controller Configuration Byte
    // Bit 0: KB Interrupt, Bit 1: Mouse Interrupt, Bit 6: Translation
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_READ_CONFIG);
    ps2_wait_read();
    uint8_t status = inb(PS2_DATA_PORT);
    status |= (1 << 0) | (1 << 1); // Enable IRQ 1 and IRQ 12
    
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_WRITE_CONFIG);
    ps2_wait_write();
    outb(PS2_DATA_PORT, status);

    // 4. Enable Devices
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_ENABLE_KB);
    ps2_wait_write();
    outb(PS2_COMMAND_REG, PS2_CMD_ENABLE_MS);

    // 5. Initialize Mouse (The mouse won't send IRQs until you tell it to)
    ps2_write_mouse(MOUSE_CMD_SET_DEFAULTS);
    ps2_wait_read(); inb(PS2_DATA_PORT); // Read ACK (0xFA)
    
    ps2_write_mouse(MOUSE_CMD_ENABLE_SCAN);
    ps2_wait_read(); inb(PS2_DATA_PORT); // Read ACK (0xFA)
}

/* --- IRQ Handlers --- */

// Called from IRQ 1 (Keyboard)
void ps2_keyboard_handler(void) {
    uint8_t scancode = inb(PS2_DATA_PORT);
    // Process scancode (e.g., put it into a circular buffer)
}

// Called from IRQ 12 (Mouse)
static uint8_t mouse_cycle = 0;
static uint8_t mouse_bytes[3];

void ps2_mouse_handler(void) {
    uint8_t status = inb(PS2_STATUS_REG);
    
    // Ensure this is actually mouse data
    if (!(status & PS2_STATUS_MOUSE)) return;

    mouse_bytes[mouse_cycle++] = inb(PS2_DATA_PORT);

    if (mouse_cycle == 3) {
        mouse_cycle = 0;
        
        // Byte 0: Flags (Buttons, Signs)
        // Byte 1: X Delta
        // Byte 2: Y Delta
        
        mouse_state_t state;
        state.left_button   = (mouse_bytes[0] & 0x01);
        state.right_button  = (mouse_bytes[0] & 0x02);
        state.middle_button = (mouse_bytes[0] & 0x04);

        // Handle negative deltas (signed 9-bit logic)
        state.x_delta = (int8_t)mouse_bytes[1];
        state.y_delta = (int8_t)mouse_bytes[2];

        // Update your kernel's internal mouse position here
    }
}
