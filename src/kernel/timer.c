#include <stdint.h>
#include "timer.h"

// Function to send a byte to an I/O port
void outb(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %b, %w" : : "a" (data), "d" (port));
}

// Function to initialize the timer with a desired frequency
void init_timer(uint32_t frequency) {
  // Calculate the divisor based on the desired frequency
  uint16_t divisor = 1193180 / frequency;

  // Send the mode and divisor byte to the timer control port
  outb(IOPORT_TIMER_CONTROL, TIMER_CTRL_SELECT_0 | TIMER_MODE_3 | TIMER_CTRL_BINARY);
  outb(IOPORT_TIMER_0, (uint8_t) (divisor & 0xFF));
  outb(IOPORT_TIMER_0, (uint8_t) ((divisor >> 8) & 0xFF));
}

// Function to wait for a specified number of microseconds
void wait_us(uint32_t microseconds) {
  uint32_t count = microseconds * 10; // Adjust for desired resolution (1 count = 100ns)
  while (count--) {
    // Implement a short delay loop here (e.g., empty loop or reading the PIT)
  }
}
