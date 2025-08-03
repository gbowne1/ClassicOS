; ==============================================================================
; boot.asm - First Stage Bootloader (CHS Based)
; ==============================================================================

[BITS 16]
[ORG 0x7C00]

start:
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

    ; Load second-stage bootloader (boot1.asm) to 0x7E00
    mov si, 0x7E00              ; Destination offset (ES already zero, so physical 0x7E00)
    mov al, 1                   ; Number of sectors to read (adjust if needed)
    mov cl, 1                   ; Starting sector (CHS - sector number, 1-based)
    call read_chs
    jc disk_error               ; Jump if carry set (read error)

    ; Load kernel to 0x100000 (1 MB)
    mov si, 0x0000              ; Destination offset
    mov ax, 0x1000              ; ES = 0x1000 (0x1000:0x0000 = 1 MB)
    mov es, ax
    xor bx, bx
    mov al, 16                  ; Number of sectors for kernel (example)
    mov cl, 2                   ; Starting sector (adjust as per your disk layout)
    call read_chs
    jc disk_error

    ; Memory Validation: Verify checksum of second stage bootloader
    mov si, 0x7E00             ; Start of second stage
    mov cx, 512                ; Size in bytes (adjust if more sectors loaded)
    call verify_checksum
    jc disk_error              ; Jump if checksum fails

    ; Enable A20 line
    call enable_a20
    jc a20_error                ; Jump if A20 enable fails

    ; Setup Global Descriptor Table
    call setup_gdt

    ; Switch to protected mode and jump to second stage at 0x08:0x7E00
    call switch_to_pm

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

; ----------------------------------------------------------------
; CHS Disk Read Routine
; AL = number of sectors
; CL = starting sector (1-based)
; SI = destination offset (Segment:ES already set)
read_chs:
    pusha
    push dx
    mov cx, 5
.retry:
    mov ah, 0x02             ; BIOS read sector
    mov dl, [bootdev]        ; Drive number
    int 0x13

    jc .error                ; Carry flag set = error
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
    ; Fast A20 gate method
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ; Fallback method (keyboard controller)
    jc .fallbackenable_a20:
    ; Fast A20 method
    in al, 0x92
    or al, 0x02
    and al, 0xFE  ; Clear bit 0 to avoid fast A20 issues on some systems
    out 0x92, al
    ; Verify A20
    call check_a20
    jnc .done
    ; Fallback method
    mov al, 0xAD
    out 0x64, al
    ; ... (rest of keyboard controller code)
.done:
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

.fallback:
    mov al, 0xAD
    out 0x64, al
    mov al, 0xD0
    out 0x64, al
    in al, 0x60
    or al, 0x02
    mov al, 0xD1
    out 0x64, al
    mov al, 0xAE
    out 0x64, al
    ret

; ----------------------------------------------------------------
gdt_start:
    dq 0x0000000000000000         ; Null descriptor
    dq 0x00CF9A000000FFFF         ; 32-bit code segment (selector 0x08)
    dq 0x00CF92000000FFFF         ; 32-bit data segment (selector 0x10)
    dq 0x00009A000000FFFF         ; 16-bit code segment for real mode (selector 0x18)

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_end:

setup_gdt:
    lgdt [gdt_descriptor]
    ret

; ----------------------------------------------------------------
switch_to_pm:
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:0x7E00

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
sectors_per_track dw 63
heads_per_cylinder dw 255

times 510 - ($ - $$) db 0
dw 0xAA55
