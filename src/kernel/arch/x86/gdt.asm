global LoadGDT

section .text
    LoadGDT:

		mov eax, [esp+4]  ; Get the pointer to the GDT pointer structure
    	lgdt [eax]        ; Load the GDT
    	mov ax, 0x10      ; Set the kernel data segment selector
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
    	mov ss, ax        ; Set the kernel stack segment selector
    	jmp 0x08:.flush   ; Flush the instruction cache

		.flush:
    		ret