#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

void irq_remap(void);
void irq_install(void);
void irq_handler(uint32_t int_num);

#endif
