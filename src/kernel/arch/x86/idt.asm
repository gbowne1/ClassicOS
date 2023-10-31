global LoadIDT

LoadIDT:
    LIDT [ESP + 32]
    RET