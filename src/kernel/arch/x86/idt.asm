global LoadIDT

section .text
LoadIDT:
mov eax, [offset idt] ; Get the IDT address
    LIDT [eax + 8*0x33]   ; Load only the timer interrupt entry (offset 8*0x33)
    RET