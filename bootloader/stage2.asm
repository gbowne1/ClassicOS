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

; ----------------------------------------------------------------------------
; ATA read sectors (LBA mode) 
;
; sysv32 abi signature:
; void ata_lba_read(uint32_t lba, uint8_t nsect, void *addr);
; ----------------------------------------------------------------------------
ata_lba_read:
    push ebp
    mov ebp, esp

    push ebx
    push ecx
    push edx
    push edi

    ; Wait BSY=0 before proceeding to write the regs
.wait_rdy:
    mov edx, 0x1F7
    in al, dx
    test al, 0x80
    jnz .wait_rdy

    mov eax, [ebp+8]     ; arg #1 = LBA
    mov cl, [ebp+12]     ; arg #2 = # of sectors
    mov edi, [ebp+16]    ; arg #3 = buffer address
    and eax, 0x0FFFFFFF

    mov ebx, eax         ; Save LBA in RBX

    mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al

    mov edx, 0x01F2      ; Port to send number of sectors
    mov al, cl           ; Get number of sectors from CL
    out dx, al

    mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
    mov eax, ebx         ; Get LBA from EBX
    out dx, al

    mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al

    mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al

    mov edx, 0x1F7       ; Command port
    mov al, 0x20         ; Read with retry.
    out dx, al

    mov bl, cl           ; Save # of sectors in BL

.wait_rdy2:
    mov edx, 0x1F7
.do_wait_rdy2:
    in al, dx
    test al, 0x80        ; BSY?
    jnz .do_wait_rdy2
    test al, 0x8         ; DRQ?
    jz .do_wait_rdy2

    mov edx, 0x1F0       ; Data port, in and out
    mov ecx, 256
    rep insw             ; in to [RDI]

    dec bl               ; are we...
    jnz .wait_rdy2       ; ...done?

    pop edi
    pop edx
    pop ecx
    pop ebx
    pop ebp
    ret
