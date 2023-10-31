global LoadGDT

section .text
    LoadGDT:
        LGDT [ESP + 32]
        RET