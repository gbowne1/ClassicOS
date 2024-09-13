#!/bin/sh
set -e

# TODO: Check the compiler does x86

if [ "$1" = "build" ]; then
    shift

    printf "Assembling bootloader...\n"
    nasm -f elf32 boot.asm -o boot.o

    printf "Compiling...\n"
    gcc \
        -save-temps \
        -std=c99 -m32 \
        -fno-pic \
        -mgeneral-regs-only \
        -ffreestanding -nostdlib \
        -Wall -Wextra -Wpedantic \
        kernel.c boot.o \
        -o kernel.bin \
        -T linker.ld
fi

if [ "$1" = "boot" ]; then
    printf "Booting...\n\n"
    MACHINE="-machine pc -cpu 486"
    qemu-system-i386 $MACHINE -net none -serial stdio -drive file=kernel.bin,index=0,if=floppy,format=raw
fi