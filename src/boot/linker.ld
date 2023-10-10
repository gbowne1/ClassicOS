ENTRY(start)
SECTIONS {
    . = 0x7c00;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /* Define the bootloader signature at the end of the bootloader */
    bootloader_signature : {
        *(.bootloader_signature)
    }

    /* Define the bootloader magic number at the end of the bootloader */
    bootloader_magic : {
        *(.bootloader_magic)
    }

    /* Define the bootloader padding to fill up the remaining space */
    bootloader_padding : {
        *(.bootloader_padding)
    }

    /* Set the bootloader size to 512 bytes */
    = 512;
}