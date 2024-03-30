; Define constants (adjust as needed)
boot_drive equ 0x00   ; Drive to load kernel from (usually 0 for primary)
kernel_sector equ 1   ; Sector containing the kernel (adjust for your kernel's location)
kernel_segments equ 4  ; Number of sectors to load (adjust for your kernel size)
kernel_load_address equ 0x1000 ; Memory address to load the kernel

; ... (Function prototypes and int_13h implementation copied from previous response)
; Function prototypes for readability
; (These functions are not strictly necessary in NASM, but improve code organization)
; Prototype for BIOS disk read interrupt (INT 13h)
void int_13h(unsigned int ah, unsigned int al, unsigned int dx, unsigned int ch, unsigned int cl, unsigned int bx);

; Function prototype for error handling or printing a message
void error_handler(const char *message);
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