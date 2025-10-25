AS = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386
IMG_SIZE = 1440k

BUILD_DIR = build

BOOT_SRC = bootloader/boot.asm
BOOT_BIN = $(BUILD_DIR)/boot.bin
BOOT_IMG = $(BUILD_DIR)/boot.img
KERNEL_SRC = kernel/kmain.c
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
DISK_IMG = $(BUILD_DIR)/disk.img

all: $(BOOT_IMG) $(KERNEL_BIN) $(DISK_IMG)

stage1: $(BOOT_IMG)

$(BUILD_DIR):
	mkdir -p $@

$(BOOT_BIN): $(BOOT_SRC) | $(BUILD_DIR)
	$(AS) -f bin -o $@ $<

$(BOOT_IMG): $(BOOT_BIN)
	cp $(BOOT_BIN) $@
	truncate -s $(IMG_SIZE) $@

$(KERNEL_BIN): $(KERNEL_SRC) | $(BUILD_DIR)
	$(CC) -ffreestanding -c $< -o $(BUILD_DIR)/kernel.o
	$(LD) -T bootloader/linker.ld -o $@ $(BUILD_DIR)/kernel.o

$(DISK_IMG): $(BOOT_IMG) $(KERNEL_BIN)
	dd if=$(BOOT_IMG) of=$@ bs=512 seek=4
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=200

run: $(DISK_IMG)
	$(QEMU) -drive file=$<,format=raw,if=floppy

clean:
	rm -rf $(BUILD_DIR)
