section .boot align=512
bits 16
global boot
boot:
	mov ax, 0x2401
	int 0x15

	mov ax, 0x3
	int 0x10

	mov [disk],dl

	mov ah, 0x2    ;read sectors
	mov al, 64     ;sectors to read
	mov ch, 0      ;cylinder idx
	mov dh, 0      ;head idx
	mov cl, 2      ;sector idx
	mov dl, [disk] ;disk idx
	mov bx, copy_target;target pointer
	int 0x13
	cli
	lgdt [gdt_pointer]
	mov eax, cr0
	or eax,0x1
	mov cr0, eax
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp CODE_SEG:boot2

; access(8)
;    P:present(1) DPL:descriptor_privilege_level(2)
;    S:descriptor_type(1) E:executable(1) DC:direction/conforming(1)
;    RW:readable/writable(1) A:accessed(1)
; flags(4)
;    G:granularity(1) DB:size(1) L:long_mode_code(1) Reserved(1)

gdt_start:
	dq 0x0000000000000000 ; null descriptor

gdt_code:
	dw 0xFFFF    ; limit_0_15(16)
	dw 0x0000    ; base_0_15(16)
	db 0x00      ; base_16_23(8)
	db 10011010b ; P(1)=1=present DPL(2)=00=ring0 S(1)=1=non_system E(1)=1=code
	             ; DC(1)=0=same_ring RW(1)=1=readable A(1)=0=not_accessed
	db 11001111b ; G(1)=1=page, DB(1)=1=32b, L(1)=0=non_64b Reserved(1)=0=_
	             ; limit_16_19(4)=1111
	db 0x00      ; base_24_31(8)

gdt_data:
	dw 0xFFFF    ; limit_0_15(16)
	dw 0x0000    ; base_0_15(16)
	db 0x00      ; base_16_23(8)
	db 10010010b ; P(1)=1=present DPL(2)=00=ring0 S(1)=1=non_system E(1)=0=data
	             ; DC(1)=0=grows_up RW(1)=1=rw A(1)=0=not_accessed
	db 11001111b ; G(1)=1=page DB(1)=1=32b_sp L(1)=0=_ Reserved(1)=0=_
	             ; limit_16_19(4)=1111
	db 0x00      ; base_24_31(8)

gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
disk:
	db 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

copy_target equ kernel_stack_bottom + 16384
hello: db "Hello more than 512 bytes world!!",0

times 510 - ($-$$) db 0
dw 0xaa55
boot2:
	mov esi,hello
	mov ebx,0xb8000
.loop:
	lodsb
	or al,al
	jz halt
	or eax,0x0F00
	mov word [ebx], ax
	add ebx,2
	jmp .loop
halt:
	mov esp,kernel_stack_top
	extern kmain
	call kmain
	cli
	hlt

section .bss
align 4
kernel_stack_bottom: equ $
	resb 16384 ; 16 KB
kernel_stack_top: equ kernel_stack_bottom + 16384