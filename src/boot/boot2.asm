[BITS 16]
[ORG 0x0000]

start:
    MOV AX, CS
    MOV DS, AX
    MOV SS, AX
    MOV SP, 0xFFFF

    MOV SI, msg
    JMP println

    JMP $

    RET

println:
    .next_char:
        LODSB
        OR AL, AL
        JZ .stop

        MOV AH, 0x0E
        MOV BH, 0x00
        MOV BL, 0x07
        INT 0x10

        JMP .next_char

    .stop:
        MOV AH, 0x03  ; AH = 0x03 (Get Cursor Position and Shape)
        XOR BH, BH    ; BH = 0x00 (Video Page Number)
        INT 0x10      ; Call video interrupt

        INC DH

        MOV AH, 0x02  ; Set cursor position
        XOR BH, BH    ; Page number
        XOR DL, DL    ; Column (start from 0)
        INT 0x10      ; Call video interrupt
        RET

msg db "Second stage loaded, and Gbowne1 stinks.", 0