#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required commands
for cmd in nasm gcc qemu-system-i386; do
    if ! command_exists "$cmd"; then
        echo "Error: $cmd is not installed or not in PATH" >&2
        exit 1
    fi
done

# Compile the assembly file
echo "Compiling boot4.asm..."
nasm -f elf32 boot4.asm -o boot4.o

# Compile and link the kernel
echo "Compiling and linking kernel..."
gcc -m32 -ffreestanding -nostdlib kernel.c boot4.o -o kernel.bin -T linker.ld

# Check if compilation was successful
if [ -f kernel.bin ]; then
    echo "Build successful. kernel.bin created."

    # Ask user if they want to run QEMU
    read -p "Do you want to run QEMU now? (y/n) " -n 1 -r
    echo    # Move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "Running QEMU..."
        qemu-system-i386 -enable-kvm -net none -fda kernel.bin
    else
        echo "QEMU not started. You can run it later with:"
        echo "qemu-system-i386 -enable-kvm -net none -fda kernel.bin"
    fi
else
    echo "Build failed. Check for errors above." >&2
    exit 1
fi
