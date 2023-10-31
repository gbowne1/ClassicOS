global inb
global outb

section .text
inb:
    MOV DX, WORD [ESP + 16]
    IN AL, DX
    RET

outb:
    MOV DX, WORD [ESP + 16]
    MOV AL, BYTE [ESP + 24]
    OUT DX, AL
    RET