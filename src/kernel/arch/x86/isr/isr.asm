global isr_handler
global _isr_stub

_isr_stub:
    PUSH DWORD [ESP + 8]
    PUSH DWORD [ESP + 8]

    CALL isr_handler

    ADD ESP, 8
    IRETD