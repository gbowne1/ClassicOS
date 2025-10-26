[BITS 32]
GLOBAL isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9
GLOBAL isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19
GLOBAL isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29
GLOBAL isr30, isr31, isr_default

[EXTERN isr_handler]

%macro ISR_NOERR 1
isr%1:
    cli
    push dword 0         ; Dummy error code
    push dword %1        ; Interrupt number
    call isr_handler
    add esp, 8
    sti
    iret
%endmacro

%macro ISR_ERR 1
isr%1:
    cli
    push dword %1        ; Interrupt number
    call isr_handler
    add esp, 8
    sti
    iret
%endmacro

; ISR 0–7: No error code
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7

; ISR 8–14: Error code pushed automatically
ISR_ERR 8
ISR_NOERR 9           ; Coprocessor Segment Overrun (obsolete, no error code)
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14

; ISR 15 is reserved
ISR_NOERR 15

; ISR 16–19 (FPU, Alignment Check, etc.)
ISR_NOERR 16
ISR_ERR 17
ISR_NOERR 18
ISR_NOERR 19

; ISR 20–31 (reserved or future use)
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; Fallback handler
isr_default:
    cli
    push dword 255
    push dword 0
    call isr_handler
    add esp, 8
    sti
    iret
