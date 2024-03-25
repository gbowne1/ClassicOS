[BITS  16]
[ORG  0x0000]

start:
    ; Initialize stack
    MOV AX, 0x0000 ; Set up a segment for the stack
    MOV SS, AX
    MOV SP, 0xFFFF ; Stack grows downwards from the top of the segment

    ; Copy boot sector data to a safe location
    call mCopyBootSector
    ; Load the kernel
    CALL load_kernel

switch_to_protected_mode:
	CLI             ; Disable interrupts
	lgdt [gdt_descriptor] ; Load the global descriptor table
	MOV EAX, CR0
	OR EAX, 0x1     ; Set the PE (Protection Enable) bit
	MOV CR0, EAX
	; Far jump to flush CPU queue after changing to protected mode
	JMP CODE_SEG:init_pm ; CODE_SEG is the segment selector for code segment in GDT
init_pm:
	; Update segment registers here
	; Set code segment register (CS) to point to code segment descriptor (selector 1)
	mov ax, 0x0001
	mov ds, ax ; Set data segment register (DS) to point to data segment descriptor (selector 2)
	mov es, ax ; Set other segment registers (ES, SS, etc.) as needed
	RET

enable_a20:
	cli             ; Disable interrupts to prevent interference
	call a20wait    ; Wait for the keyboard controller to be ready
	mov al, 0xAD    ; Command to disable keyboard
	out 0x64, al    ; Send command to keyboard controller command port
	call a20wait    ; Wait for the keyboard controller to be ready
	mov al, 0xD0    ; Command to read output port
	out 0x64, al    ; Send command to keyboard controller command port
	call a20wait    ; Wait for the keyboard controller to be ready
	in al, 0x60     ; Read current state of output port
	or al, 0x02     ; Set A20 bit
	out 0x64, al    ; Send command to keyboard controller command port
	call a20wait    ; Wait for the keyboard controller to be ready
	mov al, 0xD1    ; Command to write output port
	out 0x64, al    ; Send command to keyboard controller command port
	call a20wait    ; Wait for the keyboard controller to be ready
	mov al, 0xAE    ; Command to re-enable keyboard
	out 0x64, al    ; Send command to keyboard controller command port
	sti             ; Re-enable interrupts
	ret

; Wait for keyboard controller to be ready
a20wait:
	in al, 0x64         ; Read keyboard controller status port
	test al, 0x02       ; Check if input buffer is full
	jnz a20wait         ; Wait until it's not full
	ret

; Enter kernel space and jump to the kernel entry point
JMP  0x1000:0x0000

; Code to set up flat memory model for protected mode
; This involves setting up the segment registers with selectors
; that point to descriptors in the GDT that define a flat memory model

limit = 0x00CFFFFFh  ; Define limit as a separate variable within gdt_struct

; Placeholder instruction to satisfy NASM
dummy_instruction DB 0x90  ; NOP instruction as a placeholder

gdt_struct:

    base_addr equ 0x0000000

    ; Null descriptor (ignored)
    dd base_addr, 0  ; Both values are zero for a null descriptor

    ; Code segment descriptor (flat memory model)
    dd base_addr, limit
    db 0x9A
    db 0xCF

    ; Data segment descriptor (flat memory model)
    dd base_addr, limit
    db 0x92
    db 0xCF

; Macro to copy boot sector data to a safe location
mCopyBootSector:
    pusha                       ; Save all general-purpose registers
    mov si, 0x7C00              ; Source address: where BIOS loads the boot sector
    mov di, 0x6000              ; Destination address: safe memory area
    mov cx, 512                 ; Number of bytes to copy (size of boot sector)
    cld                         ; Clear direction flag to increment SI and DI
copy_loop:
    lodsb                       ; Load byte at address DS:SI into AL, increment SI
    stosb                       ; Store byte from AL to address ES:DI, increment DI
    loop copy_loop              ; Decrement CX; if CX != 0, repeat loop
    popa                        ; Restore all general-purpose registers
    ret

; Subroutine to load the kernel
load_kernel:
    ; Disable interrupts
    cli

    ; Setup disk parameters
    ; ... (set CH, CL, DH, DL for LBA, set DX for drive number)

    ; Calculate load address for kernel
    ; ... (set ES:BX to the target memory address)

    ; Read sectors from disk into memory
    mov ah,  0x02 ; Read sectors function
    mov al,  1    ; Number of sectors to read
    int  0x13     ; BIOS disk services

    ; Check for read error
    jc .disk_error ; Carry flag set means an error occurred

    ; Enable A20 line if necessary
    ; ... (implementation depends on your system)

    ; Jump to the kernel's entry point
    jmp  0x1000:0x0000 ; Assuming the kernel is loaded at  0x1000:0x0000

.disk_error:
    ; Handle disk read error
    ; ... (display error message or halt)

    hlt ; Halt the system

; Function or Subroutine to switch to protected mode
switch_to_protected_mode:
    CLI             ; Disable interrupts
    LGDT [gdt_descriptor] ; Load the global descriptor table
    MOV EAX, CR0
    OR EAX,  0x1     ; Set the PE (Protection Enable) bit
    MOV CR0, EAX
    ; Far jump to flush CPU queue after changing to protected mode
    JMP CODE_SEG:init_pm ; CODE_SEG is the segment selector for code segment in GDT