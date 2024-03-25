;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stage 2 bootloader for ClassicOS ;
; -------------------------------- ;
; Loads the kernel, sets up tables, ;
; and transitions to protected mode ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;
; Assembler directives ;
;;;;;;;;;;;;;;;;;;;;;;;;

; tells the assembler that the program will be loaded at 0x8000
; this is done by the first stage bootloader
org 0x8000

; we are targeting (x86) 16-bit real mode
bits 16

;;;;;;;;;;;;;;;;;
; Jump to start ;
;;;;;;;;;;;;;;;;;
jmp start

;;;;;;;;
; Data ;
;;;;;;;;
; kernel file name
kername db "KERNEL.BIN", 0

;;;;;;;;;;;
; Program ;
;;;;;;;;;;;

start:
   xor ax, ax             ; set up segment registers to segment 0 since
   mov ds, ax             ; our addresses are already relative to 0x8000
   mov es, ax

   mov si, kername         ; print kernel file name
   call printstr

   ; Load the kernel into memory
   ; ...

   ; Set up GDT, IDT, IVT
   ; ...

   ; Switch to protected mode
   ; ...

   ; Set up stack and start executing kernel's code
   ; ...

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; printstr routine, prints the string pointed by si using int 0x10 ;
; sets the direction flag to 0                                     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printstr:
   cld                    ; clear df flag - lodsb increments si
printstr_loop:
   lodsb                 ; load next character into al, increment si
   or al, al              ; sets zf if al is 0x00
   jz printstr_end
   mov ah, 0x0E           ; teletype output (int 0x10)
   int 0x10               ; print character
   jmp printstr_loop
printstr_end:
   ret                    ; return to caller address

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; halt routine - infinite loop ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
halt:
   cli
   jmp halt

;;;;;;;;;;;
; Padding ;
;;;;;;;;;;;
; $ is the address of the current line, $$ is the base address for
; this program.
; the expression is expanded to 510 - ($ - 0x8000), or
; 510 + 0x8000 - $, which is, in other words, the number of bytes
; before the address 510 + 0x8000 (= 0x80FD), where the 0xAA55
; signature shall be put.
times 510 - ($ - $$) db 0x00

;;;;;;;;;;;;;;;;;;
; BIOS signature ;
;;;;;;;;;;;;;;;;;;
dw 0xAA55
