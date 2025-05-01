[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    call enable_a20
    call setup_gdt
    call switch_to_pm

; ----------------------
; A20 Gate Enable (Fast method)
enable_a20:
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ret

; ----------------------
; Set up a minimal GDT
gdt_start:
    dq 0x0000000000000000          ; null descriptor
    dq 0x00CF9A000000FFFF          ; code segment descriptor
    dq 0x00CF92000000FFFF          ; data segment descriptor

gdt_descriptor:
    dw gdt_end - gdt_start - 1     ; size of GDT
    dd gdt_start                   ; address of GDT

gdt_end:

setup_gdt:
    lgdt [gdt_descriptor]
    ret

; ----------------------
; Switch to Protected Mode
switch_to_pm:
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_mode_entry
; ----------------------
; 32-bit Protected Mode Entry Point
[BITS 32]
protected_mode_entry:
    mov ax, 0x10       ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Kernel is assumed to be loaded at 0x100000
    jmp 0x08:0x100000

; ----------------------
; Boot signature
times 510 - ($ - $$) db 0
dw 0xAA55
