.global ctx_switch

; void ctx_switch(uint32_t **old_sp_ptr, uint32_t *new_sp);
; Arguments on stack (cdecl convention):
; [ESP + 4] -> old_sp_ptr (pointer to the 'stack_ptr' field of current task)
; [ESP + 8] -> new_sp     (value of 'stack_ptr' of the next task)

ctx_switch:
    ; 1. Save the context of the CURRENT task
    pushf               ; Save EFLAGS (CPU status flags)
    pusha               ; Save all General Purpose Regs (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)

    ; 2. Save the current stack pointer (ESP) into the pointer passed as 1st arg
    mov eax, [esp + 40] ; Get 1st argument (old_sp_ptr). Offset 40 = 36 (regs) + 4 (ret addr)
    mov [eax], esp      ; *old_sp_ptr = ESP

    ; 3. Load the stack pointer of the NEW task
    mov esp, [esp + 44] ; Get 2nd argument (new_sp). Offset 44 = 40 + 4

    ; 4. Restore the context of the NEW task
    popa                ; Restore all General Purpose Regs
    popf                ; Restore EFLAGS

    ; 5. Jump to the new task (The 'ret' pops EIP from the new stack)
    ret
