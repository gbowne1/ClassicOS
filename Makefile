AS = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386
IMG_SIZE = 1440k

BUILD_DIR = build

BOOT_SRC = bootloader/boot.asm
BOOT_OBJ = $(BUILD_DIR)/boot.o
BOOT_ELF = $(BUILD_DIR)/boot.elf
BOOT_IMG = $(BUILD_DIR)/boot.img
KERNEL_SRC = kernel/kmain.c
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
DISK_IMG = $(BUILD_DIR)/disk.img

all: $(BOOT_IMG) $(KERNEL_BIN) $(DISK_IMG)

$(BUILD_DIR):
	mkdir -p $@

$(BOOT_OBJ): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) -f elf32 -g -F dwarf -o $@ $<

$(BOOT_ELF): $(BOOT_OBJ)
	$(LD) -Ttext=0x7c00 -melf_i386 -o $@ $<

$(BOOT_IMG): $(BOOT_ELF)
	objcopy -O binary $< $@
	truncate -s $(IMG_SIZE) $@

$(KERNEL_BIN): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) -ffreestanding -c $< -o $(BUILD_DIR)/kernel.o
	$(LD) -T bootloader/linker.ld -o $@ $(BUILD_DIR)/kernel.o

$(DISK_IMG): $(BOOT_IMG) $(KERNEL_BIN)
	dd if=$(BOOT_IMG) of=$@ bs=512 seek=4
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=200

run: $(DISK_IMG)
	$(QEMU) -drive file=$<,format=raw,if=floppy

.PHONY: stage1 clean

stage1: $(BOOT_IMG)

clean:
	rm -rf $(BUILD_DIR)
