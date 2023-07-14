 ENTRY(main)

SECTIONS {
  . = 0x7c00;

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
  }
}