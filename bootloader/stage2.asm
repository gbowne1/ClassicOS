[BITS 32]
global _start
global ata_lba_read

extern load_kernel

_start:
    ; Set up segments
    ; Data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Code segment
    mov ax, 0x08
    mov cs, ax

    ; Stack (must be identity-mapped)
    mov esp, 0x90000

    call load_kernel

    jmp eax
