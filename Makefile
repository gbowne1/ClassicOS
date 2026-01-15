AS = nasm
ASFLAGS = -f elf32 -g -F dwarf
CC = i386-elf-gcc
LD = i386-elf-ld
QEMU= qemu-system-i386
OBJCOPY = i386-elf-objcopy

BUILD_DIR = build
CROSS_DIR = cross
DISK_IMG = $(BUILD_DIR)/disk.img

STAGE2_ADDR = 0x7e00
STAGE2_SIZE = 2048

# Place the memory map (e820) past stage2 bl in memory
MEMMAP_BASE = $(shell echo $$(($(STAGE2_ADDR) + $(STAGE2_SIZE))))

KERNEL_C_SRC = $(wildcard kernel/*.c)
KERNEL_ASM_SRC = $(wildcard kernel/*.asm)
KERNEL_OBJ = $(patsubst kernel/%.c, $(BUILD_DIR)/%.o, $(KERNEL_C_SRC))
KERNEL_OBJ += $(patsubst kernel/%.asm, $(BUILD_DIR)/asm_%.o, $(KERNEL_ASM_SRC))

KLIBC_SRC = $(wildcard klibc/src/*.c)
KLIBC_OBJ = $(patsubst klibc/src/%.c, $(BUILD_DIR)/klibc/%.o, $(KLIBC_SRC))

.PHONY: all stage1 stage2 kernel compile-commands $(BUILD_DIR)/compile_commands.json run gdb clean clean-cross clean-all
all: $(DISK_IMG)

stage1: $(BUILD_DIR)
	$(AS) $(ASFLAGS) -o $(BUILD_DIR)/$@.o bootloader/$@.asm
	$(LD) -Ttext=0x7c00 -melf_i386 -o $(BUILD_DIR)/$@.elf $(BUILD_DIR)/$@.o
	$(OBJCOPY) -O binary $(BUILD_DIR)/$@.elf $(BUILD_DIR)/$@.bin

# NOTE: Stage2 final size should be checked against `$(STAGE2_SIZE)` by the build system to avoid an overflow.
# Alternatively, convey the final stage2 size through other means to stage1.
stage2: $(BUILD_DIR)
	$(AS) $(ASFLAGS) -DMEMMAP_BASE=$(MEMMAP_BASE) -o $(BUILD_DIR)/stage2.o bootloader/stage2.asm
	$(CC) -std=c11 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -m32 -Iklibc/include -g -c -o $(BUILD_DIR)/stage2_load.o bootloader/stage2_load.c
	$(LD) -Tbootloader/stage2.ld -melf_i386 -o $(BUILD_DIR)/$@.elf $(BUILD_DIR)/stage2.o $(BUILD_DIR)/stage2_load.o
	$(OBJCOPY) -O binary $(BUILD_DIR)/$@.elf $(BUILD_DIR)/$@.bin
	truncate -s $(STAGE2_SIZE) $(BUILD_DIR)/$@.bin

$(BUILD_DIR)/asm_%.o: kernel/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: kernel/%.c
	$(CC) -DMEMMAP_BASE=$(MEMMAP_BASE) -std=c11 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -m32 -Iklibc/include -g -c -o $@ $<

$(BUILD_DIR)/klibc/%.o: klibc/src/%.c
	$(CC) -std=c11 -ffreestanding -nostdlib -nostdinc -fno-stack-protector -m32 -Iklibc/include -g -c -o $@ $<

kernel: $(KERNEL_OBJ) | $(BUILD_DIR) $(KLIBC_OBJ)
	$(LD) -melf_i386 -Tkernel/linker.ld -o $(BUILD_DIR)/kernel.elf $(KERNEL_OBJ) $(KLIBC_OBJ)

$(DISK_IMG): stage1 stage2 kernel
	dd if=$(BUILD_DIR)/stage1.bin of=$@
	dd if=$(BUILD_DIR)/stage2.bin of=$@ oflag=append conv=notrunc
	dd if=$(BUILD_DIR)/kernel.elf of=$@ oflag=append conv=notrunc
	truncate -s 1M $@

$(BUILD_DIR):
	mkdir -p $@
	mkdir -p $(BUILD_DIR)/klibc

compile-commands: $(BUILD_DIR)/compile_commands.json
$(BUILD_DIR)/compile_commands.json: $(BUILD_DIR)
	bear --output $@ -- make -B

run:
	qemu-system-i386 -s -S $(DISK_IMG)

gdb:
	gdb -x gdb.txt

clean:
	rm -rf $(BUILD_DIR)

clean-cross:
	rm -rf $(CROSS_DIR)
	rm -rf .build.env

clean-all: clean clean-cross
