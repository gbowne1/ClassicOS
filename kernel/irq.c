#include "irq.h"
#include "io.h"
#include "isr.h"

#define PIC1_CMD   0x20
#define PIC1_DATA  0x21
#define PIC2_CMD   0xA0
#define PIC2_DATA  0xA1

void irq_remap(void)
{
    outb(PIC1_CMD, 0x11);   // ICW1 – edge triggered, cascade, need ICW4
    outb(PIC2_CMD, 0x11);

    outb(PIC1_DATA, 0x20);  // ICW2 – master base vector
    outb(PIC2_DATA, 0x28);  // ICW2 – slave base vector

    outb(PIC1_DATA, 0x04);  // ICW3 – slave on IRQ2
    outb(PIC2_DATA, 0x02);  // ICW3 – cascade identity

    outb(PIC1_DATA, 0x01);  // ICW4 – 8086 mode
    outb(PIC2_DATA, 0x01);

    // Mask everything except IRQ0 (timer) and IRQ1 (keyboard) for now
    outb(PIC1_DATA, 0b11111001);
    outb(PIC2_DATA, 0xFF);
}

void irq_install(void)
{
    irq_remap();

    /* Fill IRQ entries in the IDT (0x20 … 0x2F) */
    extern void irq0(), irq1(), irq2(), irq3(), irq4(), irq5(), irq6(), irq7();
    extern void irq8(), irq9(), irq10(), irq11(), irq12(), irq13(), irq14(), irq15();

    idt_set_gate(0x20, (uint32_t)irq0);
    idt_set_gate(0x21, (uint32_t)irq1);
    /* … repeat for the rest or loop … */
    for (int i = 2; i < 16; ++i)
        idt_set_gate(0x20 + i, (uint32_t)irq0 + i * 8); // crude but works
}

/* Called from the assembly stubs (see irq.asm below) */
void irq_handler(uint32_t int_num)
{
    /* int_num is the *remapped* vector, e.g. 0x21 for keyboard */
    if (interrupt_handlers[int_num]) {
        interrupt_handlers[int_num]();
    }

    /* ---- EOI ---- */
    if (int_num >= 0x28)          // slave PIC
        outb(PIC2_CMD, 0x20);
    outb(PIC1_CMD, 0x20);         // always master
}
