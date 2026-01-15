# ClassicOS 2-stage bootloader

Bootloader documentation for ClassicOS

## Disk image organization:

```
[  512 B  ] [      2048 B      ] [    Unspecified    ]
  Stage 1         Stage 2               Kernel
```

## Stage 1 (`stage1.asm`)

- Queries CHS parameters from BIOS
- Loads the second stage bootloader (2048 B) to `0x7c00`
- Enables A20
- Jumps to stage2

## Stage 2 (`stage2.asm, stage2_load.c`)

- Read and store E820 memory map from BIOS
- Sets up a GDT with descriptor entries for code and data both covering the whole 32-bit address space
- Set CR0.PE (enable protected mode)
- Set up segment registers
- Load the kernel ELF header
- Parse the program headers, and load all `PT_LOAD` segments from disk
- Jump to the kernel entry
