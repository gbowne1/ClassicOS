IMG_SIZE = 1440k
BOOT_BIN = ../build/boot.bin
KERNEL_BIN = ../build/kernel.bin
DISK_IMG = ../build/disk.img

$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	dd if=$(BOOT_BIN) of=$@ bs=512 seek=4
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=200
