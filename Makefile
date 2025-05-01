AS = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386
IMG_SIZE = 1440k

BOOT_SRC = bootloader/boot.asm
BOOT_BIN = build/boot.bin
BOOT_IMG = build/boot.img
KERNEL_SRC = kernel/kmain.c
KERNEL_BIN = build/kernel.bin
DISK_IMG = build/disk.img

all: $(BOOT_IMG) $(KERNEL_BIN) $(DISK_IMG)

$(BOOT_BIN): $(BOOT_SRC)
	$(AS) -f bin -o $@ $<

$(BOOT_IMG): $(BOOT_BIN)
	cp $(BOOT_BIN) $@
	truncate -s $(IMG_SIZE) $@

$(KERNEL_BIN): $(KERNEL_SRC)
	$(CC) -ffreestanding -c $< -o build/kernel.o
	$(LD) -T bootloader/linker.ld -o $@ build/kernel.o

$(DISK_IMG): $(BOOT_IMG) $(KERNEL_BIN)
	dd if=$(BOOT_IMG) of=$@ bs=512 seek=4
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=200

run: $(DISK_IMG)
	$(QEMU) -drive file=$<,format=raw,if=floppy

clean:
	rm -rf build
