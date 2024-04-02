; Define constants (adjust as needed)
boot_drive equ 0x00   ; Drive to load kernel from (usually 0 for primary)
kernel_sector equ 1   ; Sector containing the kernel (adjust for your kernel's location)
kernel_segments equ 4  ; Number of sectors to load (adjust for your kernel size)
kernel_load_address equ 0x1000 ; Memory address to load the kernel

void int_13h(unsigned int ah, unsigned int al, unsigned int dx, unsigned int ch, unsigned int cl, unsigned int bx);

void error_handler(const char *message)
; Main kernel loading code
mov bx, kernel_load_address  ; Set load address

; Loop to load kernel sectors
mov cx, 0                     ; Initialize counter
loop_load:
    int_13h(0x02, kernel_segments, boot_drive * 256 + kernel_sector, ch, cl, bx)  ; Read sectors
    add bx, 512 * kernel_segments  ; Update load address
    inc cx                        ; Increment counter
    cmp cx, kernel_segments        ; Check if all sectors loaded
    jne loop_load                  ; Jump back if not finished

; Success - kernel is now loaded into memory
ret      ; Return to the main bootloader code