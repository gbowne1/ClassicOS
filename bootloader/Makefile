# Makefile – ClassicOS
# ------------------------------------------------------------
# Targets:
#   all      → build the final floppy image (os.img)
#   run      → build + launch in QEMU
#   clean    → remove all generated files
# ------------------------------------------------------------

# Tools
NASM     := nasm
GCC      := i686-elf-gcc
LD       := i686-elf-ld
OBJCOPY  := i686-elf-objcopy
QEMU     := qemu-system-i386
DD       := dd
MKDIR    := mkdir -p
RM       := rm -rf

# Directories
BOOTDIR  := bootloader
KERNDIR  := kernel
OBJDIR   := obj
IMGDIR   := img

# ----------------------------------------------------------------
# Bootloader sources
# ----------------------------------------------------------------
BOOT_ASM   := $(BOOTDIR)/boot.asm $(BOOTDIR)/boot1.asm
BOOT_OBJ   := $(OBJDIR)/boot.o $(OBJDIR)/boot1.o

# ----------------------------------------------------------------
# Kernel sources (C + asm)
# ----------------------------------------------------------------
KERN_C     := $(wildcard $(KERNDIR)/*.c)
KERN_ASM   := $(wildcard $(KERNDIR)/*.asm)
KERN_OBJ   := $(patsubst $(KERNDIR)/%.c,$(OBJDIR)/%.o,$(KERN_C)) \
              $(patsubst $(KERNDIR)/%.asm,$(OBJDIR)/%.o,$(KERN_ASM))

# ----------------------------------------------------------------
# Flags
# ----------------------------------------------------------------
NASMFLAGS  := -f elf32
CFLAGS     := -std=gnu99 -ffreestanding -Wall -Wextra \
              -nostdinc -nostdlib -fno-builtin -fno-stack-protector \
              -m32 -I$(KERNDIR)
LDFLAGS    := -T $(BOOTDIR)/linker.ld -m elf_i386 -nostdlib

# ----------------------------------------------------------------
# Final artefacts
# ----------------------------------------------------------------
IMG        := $(IMGDIR)/os.img
KERNEL_BIN := $(OBJDIR)/kernel.bin
BOOT_BIN   := $(OBJDIR)/boot.bin
BOOT1_BIN  := $(OBJDIR)/boot1.bin

# ----------------------------------------------------------------
# Phony targets
# ----------------------------------------------------------------
.PHONY: all clean run dirs

# ----------------------------------------------------------------
# Default
# ----------------------------------------------------------------
all: dirs $(IMG)

# ----------------------------------------------------------------
# Create needed directories
# ----------------------------------------------------------------
dirs:
	$(MKDIR) $(OBJDIR) $(IMGDIR)

# ----------------------------------------------------------------
# 1. First-stage bootloader (512 bytes, raw binary)
# ----------------------------------------------------------------
$(OBJDIR)/boot.o: $(BOOTDIR)/boot.asm
	$(NASM) -f bin -o $@ $<

# ----------------------------------------------------------------
# 2. Second-stage bootloader (ELF → raw binary)
# ----------------------------------------------------------------
$(OBJDIR)/boot1.o: $(BOOTDIR)/boot1.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

$(BOOT1_BIN): $(OBJDIR)/boot1.o
	$(LD) -Ttext=0x7E00 --oformat binary -o $@ $<

# ----------------------------------------------------------------
# 3. Kernel (C + asm → ELF → raw binary)
# ----------------------------------------------------------------
$(OBJDIR)/%.o: $(KERNDIR)/%.c
	$(GCC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(KERNDIR)/%.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

$(OBJDIR)/kernel.elf: $(KERN_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(KERNEL_BIN): $(OBJDIR)/kernel.elf
	$(OBJCOPY) -O binary $@ $<

# ----------------------------------------------------------------
# 4. Build the final floppy image
#    • 2880 sectors (1.44 MiB floppy)
#    • sector 0  → boot (MBR)
#    • sector 1–4 → boot1 (4 × 512 = 2 KB)
#    • sector 5–20 → kernel (16 sectors = 8 KB)
# ----------------------------------------------------------------
$(IMG): $(OBJDIR)/boot.o $(BOOT1_BIN) $(KERNEL_BIN)
	$(DD) if=/dev/zero of=$@ bs=512 count=2880
	$(DD) if=$(OBJDIR)/boot.o of=$@ conv=notrunc bs=512 count=1
	$(DD) if=$(BOOT1_BIN) of=$@ conv=notrunc bs=512 seek=1 count=4
	$(DD) if=$(KERNEL_BIN) of=$@ conv=notrunc bs=512 seek=5 count=16

# ----------------------------------------------------------------
# Run in QEMU
# ----------------------------------------------------------------
run: all
	$(QEMU) -fda $(IMG) -boot a

# ----------------------------------------------------------------
# Clean
# ----------------------------------------------------------------
clean:
	$(RM) $(OBJDIR) $(IMGDIR)
