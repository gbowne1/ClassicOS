#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool terminate = false;

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#define PORT 0x3f8          // COM1

void init_serial(void)
{
    // Enable "data available" interrupt.
    outb(PORT + 1, 0x01);
}

// Use only when there _is_ something to read.
char read_serial()
{
    return inb(PORT);
}

int is_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

void write_serial(char a)
{
    while (is_transmit_empty() == 0);
    outb(PORT, a);
}

#undef PORT

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

/*
 * Convert numbers to hexadecimal chars.
 */

char nibble_to_hex(int n)
{
    if   (n>=0 && n<10) return (n+48);
    else if (n>=10 && n<16) return (n+55+32);
    else return '?';
}

void int_to_hex(char *hex, uint64_t a, size_t size)
{
    int i = 0;
    int divisor = size << 3;
    while (divisor != 0) {
        divisor -= 4;
        hex[i] = nibble_to_hex((a >> divisor) & 0xF);
        i += 1;
    }
}

void u8_to_hex (char *s, uint8_t  a) { int_to_hex(s, a, 1); }
void u16_to_hex(char *s, uint16_t a) { int_to_hex(s, a, 2); }
void u32_to_hex(char *s, uint32_t a) { int_to_hex(s, a, 4); }
void u64_to_hex(char *s, uint64_t a) { int_to_hex(s, a, 8); }

/*
 * Logging functions.
 */

// Use serial for log output.
#define PUTCHAR write_serial

// Sends buffer of a given size to the log.
void klog(char *buff, int size)
{
    for (int i=0; i<size; ++i) PUTCHAR(buff[i]);
}

// Like klog but also sends a newline character.
void klogl(char *buff, int size)
{
    klog(buff, size); PUTCHAR('\n');
}

// Sends a NUL-terminated string to the log (not including the NUL).
void klogs(const char *str)
{
    while(*str) PUTCHAR(*str++);
}

#undef PUTCHAR

// Functions that send integers to the log, in hex (without 0x prefix).
#define B hexbuff
#define D char B[16] = {0}
void klog_u8 (uint8_t  a) { D; u8_to_hex (B, a); klog(B, 2) ; }
void klog_u16(uint16_t a) { D; u16_to_hex(B, a); klog(B, 4) ; }
void klog_u32(uint32_t a) { D; u32_to_hex(B, a); klog(B, 8) ; }
void klog_u64(uint64_t a) { D; u64_to_hex(B, a); klog(B, 16); }

// These send a newline character too.
void klogl_u8 (uint8_t  a) { D; u8_to_hex (B, a); klogl(B, 2) ; }
void klogl_u16(uint16_t a) { D; u16_to_hex(B, a); klogl(B, 4) ; }
void klogl_u32(uint32_t a) { D; u32_to_hex(B, a); klogl(B, 8) ; }
void klogl_u64(uint64_t a) { D; u64_to_hex(B, a); klogl(B, 16); }
#undef D
#undef B

void demo_klog_functions(void)
{
    klogs("\nCheck; one, two, one, two.\n");
    klogl_u8(0x01);
    klogl_u16(0x2345);
    klogl_u32(0x6789abcd);
    klogl_u64(0xef0123456789abcd);
    klog_u8(0x01);
    klog_u16(0x2345);
    klog_u32(0x6789abcd);
    klog_u64(0xef0123456789abcd);
    klogs("\n0123456789abcdef0123456789abcd\n");
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#define PIC1            0x20     /* IO base address for master PIC */
#define PIC2            0xA0     /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

#define ICW1_ICW4       0x01     /* Indicates that ICW4 will be present */
#define ICW1_INIT       0x10     /* Initialization - required! */

#define ICW4_8086       0x01     /* 8086/88 (MCS-80/85) mode */

void pic_remap(int offset)
{
    uint8_t a1, a2;
    // save masks
    a1 = inb(PIC1_DATA); a2 = inb(PIC2_DATA);
    // starts the initialization sequence (in cascade mode)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); io_wait();
    // ICW2: Master PIC vector offset
    outb(PIC1_DATA, offset); io_wait();
    // ICW2: Slave PIC vector offset
    outb(PIC2_DATA, offset+8); io_wait();
    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4); io_wait();
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2); io_wait();
    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    outb(PIC1_DATA, ICW4_8086); io_wait();
    outb(PIC2_DATA, ICW4_8086); io_wait();
    // restore saved masks.
    outb(PIC1_DATA, a1); outb(PIC2_DATA, a2);
}

void pic_end_of_irq(int irq)
{
    if (irq >= 8) outb(PIC2_COMMAND, 0x20);
    outb(PIC1_COMMAND, 0x20);
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

struct gdt_descriptor
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void sgdt(struct gdt_descriptor *gdtd)
{
    __asm__ volatile ("sgdt %0" : : "m"(*gdtd) : "memory");
}

void klog_info_about_gdt(void)
{
    struct gdt_descriptor gdtd;
    sgdt(&gdtd);

    klogs("**\n GDT info:\n");
    klogl_u32(gdtd.base);
    klogl_u16(gdtd.limit);

    // For each (8 bytes long) entry in the table...
    for (uint32_t b = gdtd.base; b < gdtd.base + gdtd.limit; b += 8) {
        // ... log each byte, in hex.
        for (uint32_t i = b; i < b + 8; ++i) {
            klog_u8(*(uint8_t*)i);
        }
        klogs("\n");
    }
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

struct idt_gate_descriptor
{
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  zero;            // unused, set to 0
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
} __attribute__((packed));

#define PIC_REMAP_OFFSET 0x20

// The IDT begins with gates for the exceptions; we remap the PIC to start at
// some point after them. For the PIC IRQs that's 16 gates more. If we define
// our software interrupts... we'll have to make room.
#define IDT_NUM_GATES (PIC_REMAP_OFFSET + 16)

__attribute__((aligned(0x10)))
struct idt_gate_descriptor idt[IDT_NUM_GATES] = {0};

static inline void lidt(void)
{
    struct {
        uint16_t limit;
        void     *base;
    } __attribute__((packed)) src = { IDT_NUM_GATES * 8 - 1, idt };
    __asm__ ("lidt %0" : : "m"(src) );
}

struct interrupt_frame; // Not defined yet, but we need a pointer to it.

#define GATE_F          void (*f)(struct interrupt_frame *)
#define GATE_WITH_ERR_F void (*f)(struct interrupt_frame *, uint32_t)
#define FOR_INTR        0x8e /* Present, DPL 0, 32b interrupt gate */
#define FOR_TRAP        0x8f /* Present, DPL 0, 32b trap gate */

struct idt_gate_descriptor gate(uint8_t type_attributes, uint32_t f)
{
    return (struct idt_gate_descriptor){
        .type_attributes = type_attributes,
        .selector = 0x0008, // Our code segment in GDT, ring0 requested.
        .offset_1 = (uint16_t)(f & 0xffff),
        .offset_2 = (uint16_t)((f>>16) & 0xffff),
        .zero = 0x00
    };
}

void klog_idt_gate_update(int n)
{
    klogs(" "); klog_u8(n);
}

void set_irq_handler(size_t irq, GATE_F)
{
    idt[irq + PIC_REMAP_OFFSET] = gate(FOR_INTR, (uint32_t)f);
    klog_idt_gate_update(irq);
}

void set_exception_with_err_handler(size_t number, GATE_WITH_ERR_F)
{
    idt[number] = gate(FOR_TRAP, (uint32_t)f);
    klog_idt_gate_update(number);
}

void set_exception_handler(size_t number, GATE_F)
{
    idt[number] = gate(FOR_TRAP, (uint32_t)f);
    klog_idt_gate_update(number);
}

#undef FOR_TRAP
#undef FOR_INTR
#undef GATE_WITH_ERR_F
#undef GATE_F

void pic_clear_irq(uint8_t irq)
{
    uint16_t port;
    uint8_t value;
    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA; irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void pic_mask_all_irqs(void)
{
    outb(PIC1_DATA, 0xff); outb(PIC2_DATA, 0xff);
}

static inline void sti(void)
{
    __asm__ volatile ("sti");
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

void panic(void)
{
    klogs("Panic: halt.");
    __asm__ volatile ("cli; hlt;");
}

__attribute__((interrupt))
void exception_with_err_handler(struct interrupt_frame *frame, uint32_t error_code)
{
    (void)frame;
    klogs("\n(generic handler) Exception with err "); klogl_u32(error_code);
    panic();
}

__attribute__((interrupt))
void exception_handler(struct interrupt_frame *frame)
{
    (void)frame;
    klogs("\n(generic handler) Exception\n");
    panic();
}

__attribute__((interrupt))
void exception_de(struct interrupt_frame *frame)
{
    (void)frame;
    klogs("\nException: Divide Error.\n");
    panic();
}

__attribute__((interrupt))
void exception_gp(struct interrupt_frame *frame, uint32_t error_code)
{
    (void)frame;
    klogs("\nException: General Protection; error code "); klogl_u32(error_code);
    panic();
}

__attribute__((interrupt))
void irq1_handler(struct interrupt_frame *frame)
{
    (void)frame;
    uint8_t k = inb(0x60);
    klogs("Key: "); klogl_u8(k);
    if (k ==  /* q */ 0x90) terminate = true;
    pic_end_of_irq(1);
}

__attribute__((interrupt))
void irq4_handler(struct interrupt_frame *frame)
{
    (void)frame;
    uint8_t c = read_serial();
    write_serial(c); // Echo.

    // Testing stuff when receiving some chars.
    if (c == 'q') terminate = true;
    if (c == '0')
        __asm__ volatile ("mov $0, %bl; div %bl"); // Division by zero.
    if (c == 'f')
        __asm__ volatile ("int $13;");             // General Protection fault.

    pic_end_of_irq(4);
}

void init_interrupts(void)
{
    klogs("**\n  Init interrupts\n");

    klogs("Preparing PIC.\n");
    pic_mask_all_irqs();
    pic_remap(PIC_REMAP_OFFSET);

    klogs("Init exceptions.\n");
#define A(n) set_exception_handler(n, &exception_handler)
#define B(n) set_exception_with_err_handler(n, &exception_with_err_handler)
    set_exception_handler(0, &exception_de);
    A(1); A(2); A(3); A(4); A(5); A(6); A(7); B(8); A(9);
    B(10); B(11); B(12);
    set_exception_with_err_handler(13, &exception_gp);
    B(14); A(15); A(16); B(17); A(18); A(19);
    A(20); B(21); A(28); B(29); B(30);
#undef B
#undef A

    klogs("\nInit IRQs.\n");

    // Assign handlers to hardware interrupts.
    // Use the IRQ number here (the offset is applied elsewhere).
    set_irq_handler(1, &irq1_handler);  // Keyboard.
    set_irq_handler(4, &irq4_handler);  // Serial.
    klogs("\n");

    // Make our IDT the active one.
    lidt();

    // Unmask hardware interrupts that we're ready to handle.
    pic_clear_irq(1);
    pic_clear_irq(4);

    // Start accepting IRQs (that is... from the PIC).
    sti();
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

void kmain(void)
{
    init_serial();
    klogs("Hello from kmain!\n");
    demo_klog_functions();

    init_interrupts();

    const uint16_t color = 0x7e00;
    volatile uint16_t *vga = (volatile uint16_t *)0xb8000;
    const char *hello = "Hello! Please, see serial output.";

    for (int i = 0; hello[i] != 0; ++i) {
        vga[i + 80] = color | (uint16_t)hello[i];
    }

    klog_info_about_gdt();

    klogs("\n\n**\n  Ready!\n");
    klogs(
      "(accepting input from keyboard)\n"
      " type q to return from kmain\n"
      " key info sent to serial\n"
      "(accepting input from serial, with echo)\n"
      " send q to return from kmain\n"
      " send f to invoke GPF\n"
      " send 0 to trigger divide by zero\n"
    );
    while (!terminate) {
        __asm__ volatile ("hlt");
    }

    klogs("\nkmain returning now... o/\n");
}