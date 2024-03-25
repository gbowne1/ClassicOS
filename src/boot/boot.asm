[BITS 16]
[ORG 0x7c00]

start:
    ; Processor initialization (only stack segment register needed)
    mov    ss, 0x1000 ; Set stack segment register
	mov    sp, 0x7C00  ; Set stack pointer (SP)
    mov    ds, ss      ; Set data segment register (DS)

; Identify boot device using BIOS calls
identify_drive:
    mov     ah, 0x0E    ; Get Interrupt Vector for INT 13h (Disk Services)
    int     0x80
    cmp     cl, 0x41    ; Check for floppy drive interrupt vector (example)
    je      is_floppy
    cmp     cl, 0x80    ; Check for hard disk interrupt vector (example)
    je      is_harddrive
    ; Handle invalid drive type (error handling)
    ; ...

is_floppy:
    ; Perform floppy disk access (assuming AH=0x02 for read sectors)
    mov     ah, 0x02    ; Read sectors
    mov     al, 1       ; Number of sectors to read (1)

    ; Set CH, CL, DH, DL for floppy based on your system configuration
    ; (Replace these values with appropriate settings for your floppy drive)
    mov     ch, 0       ; Cylinder (example, adjust based on your floppy)
    mov     cl, 1       ; Sector number (example, adjust based on boot sector location)
    mov     dh, 0       ; Head number (example, typically 0 for single-sided floppies)
    mov     dl, 0x00    ; Drive number (0x00 for floppy drive A)

    ; Set ES:BX to the memory address where you want to store the read data
    mov     es, 0x1000  ; Example segment (adjust as needed)
    mov     bx, 0x0      ; Memory offset within the segment (example)

    int     0x13
    jc      error_floppy

    ; Implement error handling (omitted here for brevity)
    ; Process the read data from the floppy sector (load second stage bootloader, etc.)

is_harddrive:

	; Sample code (not guaranteed to work universally)
	mov ah, 0x02 ; Set function for read sectors
	mov al, 0x01 ; Read one sector
	; Set CH, CL, DH for desired sector location (e.g., first sector)
	mov dl, 0x80 ; Drive number (assuming drive A is boot device)
	mov es, segment_address ; Set ES for buffer to store read data
	mov bx, buffer_offset ; Set BX for offset within the buffer

	int 13h ; Raise BIOS interrupt for disk read

	; Check Carry flag for error handling
	jc harddrive_not_found ; Jump if Carry flag is set (potential error)

	; Hard drive likely present (further processing can occur)

	; ... (rest of your bootloader code)

	harddrive_not_found:
	; Handle error condition (missing drive or other issue)

	; ... (error handling logic)

memory_error:
    ; ... (error handling or continue with limited memory)

; Second stage loading (simplified example)
; Here's an improved version of the load_second_stage section with the placeholder jump replaced by actual loading logic:
; Code snippet

load_second_stage:
  ; Calculate address of second stage bootloader (assuming offset from boot sector)
  mov dx, 0x0000 ; Clear DX register for better calculation
  add dx, sector_count ; Add number of sectors to skip (adjust as needed)
  shl dx, 5 ; Multiply by sector size (512 bytes)
  add dx, 0x7C00 ; Add boot sector address offset

  ; Read second stage bootloader from calculated address
  mov ah, 0x02 ; Function for reading sectors
  mov al, 1     ; Number of sectors to read (1 for second stage)
  mov es, dx    ; Set ES segment register to calculated address
  mov bx, 0x0000 ; Set BX offset within the segment (example)

  int 13h      ; Raise BIOS interrupt for disk read

  ; Check Carry flag for error handling
  jc memory_error ; Jump if Carry flag is set (potential error)

  ; Second stage likely loaded successfully, jump to it
  jmp second_stage_address ; Direct jump to the defined address

error_floppy:
  ; Display a basic error message (optional)
  mov ah, 0x0E  ; BIOS video call for displaying text (educational purposes)
  mov bh, 0x00  ; Set background color (black)
  mov bl, 0x07  ; Set text color (white)
  mov dx, error_floppy_message ; Address of error message string
  int 0x10     ; BIOS video interrupt

  ; Halt the boot process (replace with a retry or more advanced error handling)
  hlt          ; Halt instruction

error_floppy_message db 'Floppy disk read error!', 0x0

memory_error:
    ; Check for available memory (replace with actual method)
	; ... (e.g., call BIOS service for memory size or use a constant value)
	cmp available_memory, memory_threshold ; Compare with minimum required memory
	jb limited_memory_boot ; Jump if below threshold

  	; ... (standard error handling for other scenarios)

limited_memory_boot:
  ; Perform minimal setup for limited memory boot
  ; ... (disable non-essential features, adjust kernel parameters)

  ; Load and jump to second stage bootloader (potentially with adjustments)
  ; ... (modify loading logic if necessary)

; Define variables used for calculation (adjust as needed)
sector_count db 1  ; Number of sectors to skip before second stage (change if needed)
second_stage_address equ 0x8000  ; Replace with actual address of your second stage bootloader
available_memory equ 0x100000 ; Replace with actual memory size detection (1MB in this example)
memory_threshold equ 0x0A0000 ; Example minimum memory required (adjust based on needs)

; Padding and magic number (standard practice)
times 510-($-$$) db 0
dw 0xaa55
