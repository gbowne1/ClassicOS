%include "isr.h"

section .data
    ISR_HAS_ERROR_CODE equ 1
    ISR_IS_HARDWARE_INTERRUPT equ 1

section .text
global isr_stub

global _isr_stub
_isr_stub:
    ; Save context
    PUSHAD ; Preserve all general-purpose registers

    ; Disable interrupts to handle nested interrupts
    CLI

    ; Align stack to 16-byte boundary for performance (if needed)
    AND ESP, 0xFFFFFFF0

    ; Check if an error code is present (typically in EAX)
    ; This is just a placeholder, actual implementation depends on your IDT setup
    CMP BYTE [ISR_HAS_ERROR_CODE], 1
    JNE no_error_code
    PUSH DWORD [ESP + 32] ; Push error code

no_error_code:
	PUSH DWORD 0
	MOV EAX, [ESP + 36]
	PUSH EAX
	; Call the C ISR handler
    CALL isr_handler

    MOV AL, BYTE [ISR_IS_HARDWARE_INTERRUPT]
	CMP AL, 1

	JNE skip_eoi
	MOV AL, 0x20
	OUT 0x20, AL

skip_eoi:

    POPAD
    ; Enable interrupts
    STI

    ; Clean up the stack
    ADD ESP, 16 ; Remove error code and interrupt number

    ; Return from interrupt
    IRETD