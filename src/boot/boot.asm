[BITS 16]
[ORG 0x7c00]

start:
    XOR AX, AX
    MOV DS, AX
    MOV SS, AX
    MOV SP, 0x7c00

    MOV SI, loading
    CALL printnl

    CALL reset_drives
    CALL read_drive

    MOV SI, loadedSS
    CALL printnl

    JMP 0x1000:0x0000

    RET

printnl:
    LODSB
    OR AL, AL
    JZ .stop

    MOV AH, 0x0E
    MOV BH, 0x00
    MOV BL, 0x07
    INT 0x10

    JMP printnl

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

reset_drives:
    XOR AH, AH         ; 0 = Reset floppy disk
    INT 0x13
    JC .reset_error          ; If carry flag was set, try again
    RET

    .reset_error:
        MOV SI, resetError
        CALL printnl
        RET

read_drive:
    MOV AH, 0x02
    MOV AL, 1  ; Number of sectors to read
    XOR CH, CH  ; Cylinder/Track number
    MOV CL, 2  ; Sector number (starting from 1)
    XOR DH, DH  ; Head number
    XOR DL, DL  ; Drive number (0x0 for floppy disk / 0x80 for hard disk)

    MOV BX, 0x1000
    MOV ES, BX
    XOR BX, BX

    INT 0x13

    JC .read_error
    RET

    .read_error:
        MOV SI, readError
        CALL printnl
        RET

loading db "Loading Second Stage", 0
resetError db "Failed to reset drives.", 0
readError db "Failed to read drive", 0
loadedSS db "Loaded second stage...", 0

times 510-($-$$) db 0
dw 0xaa55