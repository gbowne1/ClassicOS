#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Keyboard input buffer
#define KEYBOARD_BUFFER_SIZE 32
static uint8_t keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_head = 0;
static size_t keyboard_buffer_tail = 0;

// Keyboard interrupt handler
void keyboard_interrupt_handler() {
  uint8_t scancode = inb(0x60);

  // Add scancode to buffer
  keyboard_buffer[keyboard_buffer_head] = scancode;
  keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

// Initialize keyboard
void keyboard_init() {
  // Install keyboard interrupt handler
  install_interrupt_handler(0x09, keyboard_interrupt_handler);

  // Enable keyboard
  outb(0x64, 0xAE);
  while (inb(0x64) & 0x02);
  outb(0x60, 0xF4);
  while (inb(0x60) != 0xFA);
}

// Check if keyboard buffer is empty
bool keyboard_buffer_empty() {
  return keyboard_buffer_head == keyboard_buffer_tail;
}

// Read scancode from keyboard buffer
uint8_t keyboard_read_scancode() {
  if (keyboard_buffer_empty()) {
    return 0;
  }

  uint8_t scancode = keyboard_buffer[keyboard_buffer_tail];
  keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
  return scancode;
}