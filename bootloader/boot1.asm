; ==============================================================================
; boot1.asm - Second Stage Bootloader (Fixed Real Mode Transition)
; ==============================================================================

[BITS 32]
global _start
extern kmain

_start:
    ; Set up segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Stack (must be identity-mapped)
    mov esp, 0x90000

    ; CPU Feature Detection: check CPUID support
    pushfd                      ; Save flags
    pop eax
    mov ecx, eax
    xor eax, 1 << 21            ; Flip ID bit
    push eax
    popfd
    pushfd
    pop eax
    xor eax, ecx
    jz .no_cpuid                ; CPUID unsupported if no change

    ; CPUID supported, verify features
    mov eax, 1
    cpuid
    ; Check for paging support (bit 31 of edx)
    test edx, 1 << 31
    jz .no_paging_support

    ; Additional CPU feature checks could be added here

    jmp .cpuid_check_done

.no_cpuid:
    mov si, no_cpuid_msg
    call print_string_16
    jmp halt

.no_paging_support:
    mov si, no_paging_msg
    call print_string_16
    jmp halt

.cpuid_check_done:

    ; Temporarily switch back to real mode
    cli
    mov eax, cr0
    and eax, 0x7FFFFFFE        ; Clear PE & PG bits
    mov cr0, eax
    jmp 0x18:real_mode_entry

; ----------------------------------------------------------------
[BITS 16]
real_mode_entry:
    ; Real mode for BIOS access (E820, VESA)
    xor ax, ax
    mov es, ax

    ; VESA call
    mov di, VbeControllerInfo
    mov ax, 0x4F00
    int 0x10
    jc vesa_error

    ; E820 memory map
    xor ebx, ebx
    mov edx, 0x534D4150
    mov di, MemoryMapBuffer
    mov [MemoryMapEntries], dword 0

.e820_loop:
    mov eax, 0xE820
    mov ecx, 24
    int 0x15
    jc e820_error
    add di, 24
    inc dword [MemoryMapEntries]
    test ebx, ebx
    jnz .e820_loop
    jmp .e820_done

e820_error:
    mov si, e820_error_msg
    call print_string_16
    jmp halt

vesa_error:
    mov si, vesa_error_msg
    call print_string_16

    ; Fallback: set VGA text mode 3 and continue
    mov ah, 0x00           ; BIOS Set Video Mode function
    mov al, 0x03           ; VGA 80x25 text mode
    int 0x10

    ; Clear screen
    mov ah, 0x06           ; Scroll up function
    mov al, 0              ; Clear entire screen
    mov bh, 0x07           ; Text attribute (gray on black)
    mov cx, 0              ; Upper-left corner
    mov dx, 0x184F         ; Lower-right corner
    int 0x10

    jmp .e820_done         ; Continue booting without VESA graphics

.e820_done:
    ; Back to protected mode
    cli
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:protected_entry

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

e820_error_msg db "E820 Failed!", 0
vesa_error_msg db "VESA Failed!", 0
no_cpuid_msg db "No CPUID support detected!", 0
no_paging_msg db "CPU lacks paging support!", 0

; ----------------------------------------------------------------
[BITS 32]
protected_entry:
    ; Paging setup
    xor eax, eax
    mov edi, page_directory
    mov ecx, 1024
    rep stosd
    mov edi, page_table
    rep stosd

    mov dword [page_directory], page_table | 0x3
    mov ecx, 1024
    mov edi, page_table
    mov eax, 0x00000003
.fill_pages:
    mov [edi], eax
    add eax, 0x1000
    add edi, 4
    loop .fill_pages

    mov eax, page_directory
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    jmp kmain

; ----------------------------------------------------------------
; Data buffers and variables must be appropriately defined in your data section
MemoryMapBuffer times 128 db 0          ; 128*24 bytes reserved for E820 memory map (adjust size as needed)
MemoryMapEntries dd 0
VbeControllerInfo times 512 db 0        ; Buffer for VESA controller info (adjust size as needed)

; Define page directory and page table aligned as needed (in your data section)
align 4096
page_directory times 1024 dd 0
align 4096
page_table times 1024 dd 0

times 2047 - ($ - $$) db 0   ; Pad to the second-to-last byte of the 4th sector
checksum_byte db 0           ; This byte will be calculated and patched later
