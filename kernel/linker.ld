ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot : {
        *(.multiboot)
    }

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }

    . = ALIGN(4096);
    __stack_top = .;
    . += 128K;
    __stack_bottom = .;
}
