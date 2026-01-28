#ifndef PIC_H
#define PIC_H

#include <stdint.h>

/* I/O Ports for the PICs */
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

/* PIC Commands */
#define PIC_EOI      0x20  /* End of Interrupt */

/* Offset vectors for remapping */
#define PIC1_OFFSET  0x20
#define PIC2_OFFSET  0x28

void pic_init(void);
void pic_send_eoi(uint8_t irq);
void pic_mask(uint8_t irq);
void pic_unmask(uint8_t irq);
void pic_disable(void);

#endif
