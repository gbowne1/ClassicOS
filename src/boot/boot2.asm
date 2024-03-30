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
   %include "kernel_loader.asm"

   ; Set up IVT (GTD and IDT not used in 16 bit real mode)
   ; Define interrupt handlers (replace with your actual handler code)
   div_by_zero_handler:
	   cli
	   hlt

   timer_handler:
	   ; Your timer interrupt handler code
	   hlt
	   ret

   ; IVT entries (offset 0 for all handlers since segment 0)
   times 0x100 dw 0x0000  ; Initialize unused entries
   dw offset div_by_zero_handler  ; Interrupt 0 (Division by Zero)
   times 0x7 dw 0x0000          ; Skip unused entries
   dw offset timer_handler      ; Interrupt 0x8 (Timer)

   ; Switch to protected mode
   ; Enable Protected Mode
   switch_to_protected_mode:
     cli                   ; Disable interrupts
     mov eax, cr0           ; Get current CR0 value
     or eax, 0x01           ; Set PE bit (Protected Mode Enable)
     mov cr0, eax           ; Write modified CR0 value back

   ; Set up stack and start executing kernel's code
   ; Define GDT structure (replace with your actual GDT definition)
   gdt_start:             ; Beginning of GDT
     times 5 dd 0          ; Null descriptor entries (optional)
   gdt_code:             ; Code segment descriptor
     dw 0xffff             ; Segment size (limit)
     dw 0x0000             ; Segment base address (low)
     db 0x0              ; Segment base address (high)
     db 10011010b           ; Access rights (present, readable, conforming, executable)
     db 11001111b           ; Access rights (long mode, 4-granularity, size)
   gdt_end:               ; End of GDT
   gdt_descriptor:
     equ gdt_end - gdt_start ; Size of GDT
     dw gdt_descriptor      ; Offset of GDT
     dd gdt_start           ; Base address of GDT

   ; Load the GDT
   lgdt [gdt_descriptor]   ; Load GDT descriptor into GDTR

   ; Set up Stack (replace with your actual stack segment and address)
   mov ss, data_segment    ; Set stack segment selector
   mov esp, 0x100000       ; Set stack pointer (top of stack)

   ; Start Kernel Execution
   jmp farptr kernel_entry  ; Jump to kernel entry point (replace with actual address)

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
