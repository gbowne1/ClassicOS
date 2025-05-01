; isr.asm
[BITS 32]
[GLOBAL isr0, isr13, isr_default]

isr0:
    cli
    push byte 0
    push byte 0
    call isr_handler
    add esp, 8
    sti
    iret

isr13:
    cli
    push byte 13
    push byte 0
    call isr_handler
    add esp, 8
    sti
    iret

isr_default:
    cli
    push byte 255
    push byte 0
    call isr_handler
    add esp, 8
    sti
    iret
