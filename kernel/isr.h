#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#define MAX_INTERRUPTS 256

typedef void (*isr_callback_t)(void);
extern isr_callback_t interrupt_handlers[MAX_INTERRUPTS];

void isr_handler(uint32_t int_num, uint32_t err_code);
void register_interrupt_handler(uint8_t n, isr_callback_t handler);

#endif
