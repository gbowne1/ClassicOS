global LoadIDT  ; Declare function as global

_offset idt db 0

section .text  ; Code section

%include "idt.h"

LoadIDT:
    mov eax, [esp + 4] ; Get IDT address (ensure offset idt is defined)
    LIDT [eax]  ; Load timer interrupt entry (check for correct brackets)
    RET                 ; Return from function