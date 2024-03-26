section .text
global inb
global outb

; Read a byte from the specified port
; Input: DX = port number
; Output: AL = data read from the port
inb:
    PUSH DX    ; Preserve DX
    IN AL, DX  ; Read from port
    POP DX     ; Restore DX
    RET

; Write a byte to the specified port
; Input: DX = port number, AL = data to write
outb:
    PUSH DX    ; Preserve DX
    PUSH AX    ; Preserve AX
    MOV DX, [ESP + 6] ; Get port number from stack
    MOV AL, [ESP + 6] ; Get data from stack
    OUT DX, AL  ; Write to port
    POP AX     ; Restore AX
    POP DX     ; Restore DX
    RET