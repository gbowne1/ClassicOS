global _start
extern load_kernel

%define e820_magic 0x534d4150 ; "SMAP"
%define e820_entry_size 24
%define e820_max_entries 128

; ----------------------------------------------------------------
; Real mode
; ----------------------------------------------------------------
[BITS 16]
_start:
    call read_e820
    call setup_gdt
    call switch_to_pm

read_e820:
    xor ebx, ebx
    mov es, bx
    mov di, MEMMAP_BASE+4       ; ES=0 DI=MEMMAP_BASE+4
    xor bp, bp                  ; Keeping count in bp

.e820_loop:
    mov eax, 0xe820
    mov ecx, e820_entry_size
    mov edx, e820_magic
    int 0x15
    jc .done                    ; Error?

    cmp eax, e820_magic         ; Verify "SMAP"
    jne .done

    test ecx, ecx               ; Skip 0-sized entries
    jz .skip

    add di, e820_entry_size     ; Advance write addr
    inc bp                      ; Increment count

    cmp bp, e820_max_entries    ; Stop if we're at capacity
    jae .done
.skip:
    test ebx, ebx
    jne .e820_loop
.done:
    mov [MEMMAP_BASE], bp       ; Store count
    ret

setup_gdt:
    lgdt [gdt_descriptor]
    ret

switch_to_pm:
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:pm_entry

e820_count:
    dw 0

gdt_start:
    dq 0x0000000000000000         ; Null descriptor
    dq 0x00CF9A000000FFFF         ; 32-bit code segment (selector 0x08)
    dq 0x00CF92000000FFFF         ; 32-bit data segment (selector 0x10)
    dq 0x00009A000000FFFF         ; 16-bit code segment for real mode (selector 0x18)
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
gdt_end:

; ----------------------------------------------------------------
; Protected mode
; ----------------------------------------------------------------
[BITS 32]

pm_entry:
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

    ; Stack
    mov esp, 0x90000

    call load_kernel
    jmp eax
