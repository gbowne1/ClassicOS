global LoadIDT  ; Declare function as global

section .data
    idt_offset db 0
section .text  ; Code section

LoadIDT:
    mov eax, [esp + 4] ; Get IDT address
    lidt [eax]         ; Load IDT
    ret                ; Return from function