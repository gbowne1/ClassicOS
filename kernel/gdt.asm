; gdt.asm
; Assembler function to load the GDT and update segment registers

global gdt_flush

gdt_flush:
    mov eax, [esp + 4]   ; Argument: pointer to GDT descriptor
    lgdt [eax]           ; Load GDT

    ; Reload segment registers
    mov ax, 0x10         ; 0x10 = offset to kernel data segment (3rd entry)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Jump to flush the instruction pipeline and load CS
    jmp 0x08:.flush      ; 0x08 = offset to code segment (2nd entry)
.flush:
    ret
