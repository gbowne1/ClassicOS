;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Stage 1 bootloader for ClassicOS ;
; -------------------------------- ;
; Determines if it was loaded from ;
;  a floppy disk or an hard disk   ;
;  drive, and then loads stage 2   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;
; Assembler directives ;
;;;;;;;;;;;;;;;;;;;;;;;;

; tells the assembler that the program will be loaded at 0x7C00
; this is done by the BIOS
org 0x7C00

; we are targeting (x86) 16-bit real mode
bits 16

;;;;;;;;;;;;;;;;;
; Jump to start ;
;;;;;;;;;;;;;;;;;
jmp start

;;;;;;;;
; Data ;
;;;;;;;;
; fdd geometry & options
fddsamt  db 1        ; how many sectors to load
fddretr  db 5        ; max retries for fdd operations
fddcretr db 0        ; current retries left

; misc strings
welcome1 db "Welcome to the ClassicOS Stage 1 bootloader.", 13, 10, 0
disktype db "Drive type: ", 0
diskfdd  db "FDD", 13, 10, 0
diskhdd  db "HDD", 13, 10, 0
loaded   db "Data loaded!", 13, 10, 0

; errors
fdderes  db "FDD reset failed.", 13, 10, 0
fddeload db "FDD read failed.", 13, 10, 0

; storage
disknum  db 0

;;;;;;;;;;;
; Program ;
;;;;;;;;;;;

start:
   xor ax, ax             ; set up segment registers to segment 0 since
   mov ds, ax             ; our addresses are already relative to 0x7C00
   mov es, ax

   mov [disknum], dl      ; save disk number to memory

   mov ah, 0x01           ; set cursor shape
   mov cx, 0x0100         ; hide cursor by setting ch = 1 and cl = 0x00
   int 0x10               ; video interrupt

   mov ah, 0x08           ; read page number into bh
   int 0x10

   mov si, welcome1       ; print welcome
   call printstr

   mov si, disktype       ; print first part of disk type
   call printstr

   mov dl, [disknum]      ; restore disk number - should not be
                          ; strictly necessary but you never know
   and dl, 0x80           ; sets zf if disk is floppy
   jz fddload

hddload:
   mov si, diskhdd        ; print disk type
   call printstr
   jmp load_onto_reset

fddload:
   mov si, diskfdd        ; print disk type
   call printstr

load_onto_reset:
   mov ah, [fddretr]     ; load max retries in memory
   mov [fddcretr], ah
load_reset:
   mov si, fdderes        ; load error message pointer
   dec byte [fddcretr]    ; decrement the retries counter
   jz load_err         ; if it is 0, we stop trying

   mov ah, 0x00           ; otherwise, reset function (int 0x13)
   int 0x13
   jc load_reset       ; if jc (error), we try again

load_onto_load:
   mov ah, [fddretr]      ; reset retries counter
   mov [fddcretr], ah
   mov ax, 0x8000         ; need to stay within real mode limits
   mov es, ax
load_load:              ; loads 512*fddsamt bytes from sector 2 on.
   mov si, fddeload
   dec byte [fddcretr]
   jz load_err

   mov dh, 0              ; head 0
   mov ch, 0              ; cyl/track 0
   mov cl, 2              ; start sector
   mov bx, 0x8000              ; memory location
   mov al, [fddsamt]      ; how many sectors to read
   mov ah, 0x02           ; read function (int 0x13)
   int 0x13
   jc load_load        ; if jc (error), we try again
   cmp al, [fddsamt]      ; also if al is not 1, we have a problem
   jnz load_load

load_done:
   mov si, loaded         ; we have successfully loaded the data
   call printstr
   jmp 0x8000:0x0000               ; this will be jmp 0x1000:0x0000

load_err:
   call printstr          ; print
   jmp halt               ; and die

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; printstr routine, prints the string pointed by si using int 0x10 ;
; sets the direction flag to 0                                     ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printstr:
   cld                    ; clear df flag - lodsb increments si
printstr_loop:
   lodsb                  ; load next character into al, increment si
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
; the expression is expanded to 510 - ($ - 0x7C00), or
; 510 + 0x7C00 - $, which is, in other words, the number of bytes
; before the address 510 + 0x7C00 (= 0x7DFD), where the 0xAA55
; signature shall be put.
times 510 - ($ - $$) db 0x00

;;;;;;;;;;;;;;;;;;
; BIOS signature ;
;;;;;;;;;;;;;;;;;;
dw 0xAA55