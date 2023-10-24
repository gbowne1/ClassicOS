# Compiler and flags
CC = gcc
CXX = g++
AS = nasm
LD = ld
CFLAGS = -g -Wall
CXXFLAGS = -g -Wall -Werror -Wpedantic -Wextra
LDFLAGS = -T linker.ld

# Source files
BOOT_SOURCE_FILES = src/boot/boot.asm src/boot/linker.ld
GRUB_SOURCE_FILES = src/boot/grub/grub.cfg src/boot/grub/menu.lst
DRIVERS_SOURCE_FILES = src/drivers/audio/audio.c src/drivers/audio/audio.h \
    src/drivers/bus/eisa.c src/drivers/bus/eisa.h \
    src/drivers/bus/isa.c src/drivers/bus/isa.h \
    src/drivers/bus/mca.c src/drivers/bus/mca.h \
    src/drivers/bus/pci.c src/drivers/bus/pci.h \
    src/drivers/bus/vesa.c src/drivers/bus/vesa.h \
    src/drivers/display/display.c src/drivers/display/display.h \
    src/drivers/io/io.c src/drivers/io/io.h \
    src/drivers/keyboard/keyboard.c src/drivers/keyboard/keyboard.h \
    src/drivers/screen/screen.c src/drivers/screen/screen.h \
    src/drivers/tty/tty.c src/drivers/tty/tty.h
KERNEL_SOURCE_FILES = src/kernel/arch/x86/include/memory.h \
    src/kernel/arch/x86/include/types.h \
    src/kernel/arch/x86/memory/memory.c \
    src/kernel/arch/x86/gdt.c \
    src/kernel/arch/x86/idt.c src/kernel/arch/x86/idt.h \
    src/kernel/arch/x86/isr.c src/kernel/arch/x86/isr.h \
    src/kernel/kernel.c src/kernel/kernel.h \
    src/kernel/linker.ld src/kernel/print.c

# Object files
BOOT_OBJECT_FILES = $(patsubst %.asm, %.o, $(filter %.asm, $(BOOT_SOURCE_FILES)))
KERNEL_OBJECT_FILES = $(patsubst %.c, %.o, $(filter %.c, $(KERNEL_SOURCE_FILES)))

# Executable
ClassicOS: $(BOOT_OBJECT_FILES) $(KERNEL_OBJECT_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# Assembly files
%.o: %.asm
    $(AS) -f elf32 -o $@ $<

# C files
%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

# Clean
clean:
    rm -f $(BOOT_OBJECT_FILES) $(KERNEL_OBJECT_FILES) ClassicOS