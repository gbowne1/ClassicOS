; Set up the segment registers
xor ax, ax
mov ds, ax
mov es, ax

; Set up stack and jump to main
mov ss, ax
mov sp, 0x7c00
jmp main

; Print 'Booting ClassicOS!' to the screen
print:
   mov si, message
   mov ah, 0x0e
.loop:
   lodsb
   test al, al
   jz .done
   int 0x10
   jmp .loop
.done:
   ret

; Set the video mode to a 40 column text mode
set_video_mode:
   mov ax, 0x0003    ; Set up a 80x25 text mode
   int 0x10

   ; Set the number of columns to 40
   mov ax, 0x1112
   mov bx, 0x0007
   int 0x10

   ret

; Detect floppy disk drive
detect_disk:
   mov ah, 0x08
   int 0x13
   cmp ah, 0
   jne .error
   ret

.error:
   ; Handle disk detection error
   ; Display error message or take appropriate action
   ret

; Read sectors from floppy disk
read_sectors:
   mov ah, 0x02
   mov al, 1  ; Number of sectors to read
   mov ch, 0  ; Cylinder/Track number
   mov cl, 2  ; Sector number (starting from 1)
   mov dh, 0  ; Head number
   mov dl, 0  ; Drive number (0 for floppy disk)

   mov bx, buffer  ; Destination buffer
   mov es, bx
   xor bx, bx

   int 0x13

   jc .error  ; Check carry flag for error
   ret

.error:
   ; Handle read error
   ; Display error message or take appropriate action
   ret

; Bootloader entry point
main:
   ; Call the set_video_mode function
   call set_video_mode

   ; Clear the screen
   mov ah, 0x06
   mov al, 0
   xor bx, bx
   xor cx, cx
   mov dh, 24
   mov dl, 39
   int 0x10

   ; Call the print function
   call print

   ; Wait for a key press to exit the loop
   mov ah, 0x00
   int 0x16
   
   ; Call the detect_disk function
   call detect_disk

   ; Call the read_sectors function
   call read_sectors

   ; Infinite loop
.loop:
   jmp .loop

; Message to print
message db 'Booting ClassicOS!', 0

; Buffer to store read sectors
buffer times 512 db 0

times 510-($-$$) db 0
dw 0xaa55