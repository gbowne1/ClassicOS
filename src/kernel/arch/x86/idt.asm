global LoadIDT

section .text
LoadIDT:
    LIDT [ESP + 32]
    RET