ENTRY(kmain)

SECTIONS {
    . = 1M;

    .text : {
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : {
        *(.bss*)
        *(COMMON)
    }

    .stack (NOLOAD) : {
        . = ALIGN(4);
        . = . + 0x1000;
    }

    .heap (NOLOAD) : {
        . = ALIGN(4);
        . = . + 0x10000;
    }
}
