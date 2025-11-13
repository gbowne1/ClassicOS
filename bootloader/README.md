# ClassicOS 2-stage bootloader

Bootloader documentation for ClassicOS

## Disk image organization:

```
[  512 B  ] [      2048 B      ] [    Unspecified    ]
  Stage 1         Stage 2               Kernel
```

## Stage 1 (`stage1.asm`)

Responsible for loading the second stage using BIOS routines, and switching to protected mode.

- Queries CHS parameters from BIOS
- Loads the second stage bootloader (2048 B) to `07c00h`
- Sets up a GDT with descriptor entries for code and data both covering the whole 32-bit address space
- Enables A20
- Set CR0.PE (enable protected mode) and jump to stage 2

## Stage 2 (`stage2.asm, stage2_load.c`)

- Set up segment registers
- Load the kernel ELF header
- Parse the program headers, and load all `PT_LOAD` segments from disk
- Jump to the kernel entry
