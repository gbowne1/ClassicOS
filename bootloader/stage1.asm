; ==============================================================================
; boot.asm - First Stage Bootloader (CHS Based)
; ==============================================================================

; Params for stage2
%define s2_addr 1               ; stage2 disk offset, in sectors
%define s2_laddr 0x7e00         ; stage2 load address
%define s2_size 2048            ; stage2 size
%define s2_nsect s2_size / 512  ; stage2 size in sectors

[BITS 16]
global _start

_start:
    cli                         ; Disable interrupts

    mov [bootdev], dl           ; Save boot device number (from BIOS in DL)

    ; Setup stack safely below EBDA area (choose 0x0000:0x7A00)
    xor ax, ax                  ; AX = 0
    mov ss, ax                  ; Stack segment = 0x0000
    mov sp, 0x7A00              ; Stack offset = 0x7A00

    ; Initialize DS, ES for zero-based segments
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Query bios for disk parameters
    call get_disk_params

    ; Load second-stage bootloader (boot1.asm) to 0x7E00
    mov ax, 1                   ; LBA of boot1.asm (starts at sector 1)
    call lba_to_chs
    mov al, s2_nsect            ; Number of sectors to read
    mov bx, 0x7E00              ; Destination address offset ES = 0 (0x0000:0x7E00)
    call read_chs

    ; Enable A20 line
    call enable_a20
    jc a20_error                ; Jump if A20 enable fails

    ; Jump to s2
    jmp 0x7e00

disk_error:
    mov si, disk_error_msg
    call print_string_16
    jmp halt

a20_error:
    mov si, a20_error_msg
    call print_string_16
    jmp halt

; ----------------------------------------------------------------
; Verify Checksum Routine
; Uses SI = start address, CX = byte count
; Simple XOR checksum over bytes, expects result 0

verify_checksum:
    push ax
    push bx
    push di
    mov di, si
    xor al, al
    xor bx, bx

.verify_loop:
    lodsb
    xor bl, al
    loop .verify_loop

    test bl, bl
    jz .checksum_ok
    stc                    ; Set carry on checksum error
    jmp .done

.checksum_ok:
    clc                    ; Clear carry on success

.done:
    pop di
    pop bx
    pop ax
    ret

get_disk_params:
    mov  ah, 08h          ; BIOS: Get Drive Parameters
    int  13h
    ; TODO: error checking

    ; CL bits 0–5 contain sectors per track
    mov  al, cl
    and  al, 3Fh           ; mask bits 0–5
    mov  ah, 0
    mov  [sectors_per_track], ax

    ; DH = maximum head number (0-based)
    mov  al, dh
    inc  ax                ; convert to count (heads = maxhead + 1)
    mov  [heads_per_cylinder], ax
    ret

; ----------------------------------------------------------------
; CHS Disk Read Routine
; AL = number of sectors
; CL = starting sector (1-based)
; Inputs:
; AL = sector count
; CH = cylinder
; DH = head
; CL = sector (1–63, with top 2 bits as high cylinder bits)

; ----------------------------------------------------------------
; Convert LBA to CHS
; Inputs:
;   AX = LBA sector number (0-based)
; Outputs:
;   CH = cylinder
;   DH = head
;   CL = sector (1-63, top 2 bits are upper cylinder bits)

lba_to_chs:
    ; Sector
    xor dx, dx
    mov bx, ax
    div word [sectors_per_track] ; divide lba with max sectors
    add dl, 1 ; take the remainder, sectors start at 1
    mov cl, dl ; sector is in cl

    ; Head
    mov ax, bx
    mov dx, 0
    div word [sectors_per_track] ; divide lba with max sectors
    mov dx, 0
    div word [heads_per_cylinder] ; divide quotient with heads
    mov dh, dl ; take the remainder, head is in dh

    ; Cylinder
    mov ch, al ; take the quotient, cylinder is in ch
    ret

read_chs:
    pusha
    push dx

.retry:
    mov ah, 0x02         ; BIOS: Read sectors
    mov dl, [bootdev]    ; Boot device
    ; Assume CH, DH, CL already set before this call
    int 0x13
    jc .error
    pop dx
    popa
    ret

.error:
    dec cx
    jz disk_error
    xor ah, ah
    int 0x13
    jmp .retry

; ----------------------------------------------------------------
enable_a20:
    ; Try fast A20 gate method
    in al, 0x92
    or al, 0x02
    and al, 0xFE              ; Clear bit 0 to avoid fast A20 bugs
    out 0x92, al

    ; Verify A20
    call check_a20
    jnc .done                 ; Success

    ; Fallback: use keyboard controller method
    call .fallback

.done:
    ret

.fallback:
    mov al, 0xAD              ; Disable keyboard
    out 0x64, al
    call .wait_input_clear

    mov al, 0xD0              ; Command: read output port
    out 0x64, al
    call .wait_output_full
    in al, 0x60
    or al, 0x02               ; Set A20 enable bit
    mov bl, al

    call .wait_input_clear
    mov al, 0xD1              ; Command: write output port
    out 0x64, al
    call .wait_input_clear
    mov al, bl
    out 0x60, al

    call .wait_input_clear
    mov al, 0xAE              ; Enable keyboard
    out 0x64, al
    ret

.wait_input_clear:
    in al, 0x64
    test al, 0x02
    jnz .wait_input_clear
    ret

.wait_output_full:
    in al, 0x64
    test al, 0x01
    jz .wait_output_full
    ret

check_a20:
    in al, 0x64              ; Read keyboard controller status
    test al, 0x02            ; Check if input buffer is full
    jnz check_a20            ; Wait until empty

    mov al, 0xD0             ; Command: read output port
    out 0x64, al

.wait_output:
    in al, 0x60              ; Read output port value
    test al, 0x02            ; Check A20 gate bit (bit 1)
    jnz .a20_enabled         ; If set, A20 enabled

    xor al, al               ; Clear carry to indicate failure
    stc                      ; Set carry for failure, jc will jump

    ret

.a20_enabled:
    clc                      ; Clear carry flag to indicate success
    ret

; ----------------------------------------------------------------
print_string_16:
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .loop
.done:
    ret

disk_error_msg db "Disk error!", 0
a20_error_msg db "A20 error!", 0

halt:
    cli
    hlt

bootdev db 0
sectors_per_track dw 0
heads_per_cylinder dw 0

times 510 - ($ - $$) db 0
dw 0xAA55
