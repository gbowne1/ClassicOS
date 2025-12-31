ENTRY(kmain)

PHDRS {
    text PT_LOAD FLAGS(5);    /* Read + Execute */
    rodata PT_LOAD FLAGS(4);  /* Read only */
    data PT_LOAD FLAGS(6);    /* Read + Write */
}

SECTIONS {
    . = 1M;

    .text : {
        *(.text*)
    } :text

    .rodata : {
        *(.rodata*)
    } :rodata

    .data : {
        *(.data*)
    } :data

    .bss : {
        *(.bss*)
        *(COMMON)
    } :data

    .stack (NOLOAD) : {
        . = ALIGN(4);
        . = . + 0x1000;
    }

    .heap (NOLOAD) : {
        . = ALIGN(4);
        . = . + 0x10000;
    }
}
