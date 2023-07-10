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

       ; Infinite loop
   .loop:
       jmp .loop

   ; Message to print
   message db 'Booting ClassicOS!', 0

   times 510-($-$$) db 0
   dw 0xaa55
