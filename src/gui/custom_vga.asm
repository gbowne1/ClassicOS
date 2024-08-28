section .text
global set_custom_video_mode

set_custom_video_mode:
    mov eax, 0x4F01
    mov cx, %1
    mov dx, 0x000A0000
    mov bx, 0x8000
    int 0x10
    ret