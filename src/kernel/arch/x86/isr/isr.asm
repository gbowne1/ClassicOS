section .text
global isr_handler
global _isr_stub

_isr_stub:
    ; Save context
    PUSHAD ; Preserve all general-purpose registers

    ; Disable interrupts to handle nested interrupts
    CLI

    ; Align stack to 16-byte boundary for performance (if needed)
    ; AND ESP, 0xFFFFFFF0

    ; Check if an error code is present (typically in EAX)
    ; This is just a placeholder, actual implementation depends on your IDT setup
    CMP BYTE [ISR_HAS_ERROR_CODE], 1
    JNE no_error_code
    PUSH DWORD [ESP + 32] ; Push error code
    JMP done_pushing_error_code
no_error_code:
    PUSH DWORD 0 ; Push a dummy error code
done_pushing_error_code:

    ; Push interrupt number onto the stack
    MOV EAX, [ESP + 36] ; Move interrupt number to EAX (accounting for the dummy/error code)
    PUSH EAX ; Interrupt number

    ; Call isr_handler
    CALL isr_handler

    ; Send EOI to PIC only if it's a hardware interrupt
    ; This is just a placeholder, actual implementation depends on your IDT setup
    CMP BYTE [ISR_IS_HARDWARE_INTERRUPT], 1
    JNE skip_eoi
    MOV AL, 0x20
    OUT 0x20, AL
skip_eoi:

    ; Enable interrupts
    STI

    ; Clean up the stack
    ADD ESP, 8 ; Remove error code and interrupt number

    ; Restore context
    POPAD ; Restore all general-purpose registers

    ; Return from interrupt
    IRETD