# ClassicOS

[![Build](https://img.shields.io/badge/build-passing-brightgreen?style=flat-square)](https://github.com/gbowne1/ClassicOS/actions)
[![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-x86_IA32-lightgrey?style=flat-square)](https://en.wikipedia.org/wiki/IA-32)
[![Made with](https://img.shields.io/badge/made%20with-C%20%26%20NASM-9cf?style=flat-square)](#)

> **ClassicOS** is a 32-bit Intel x86 operating system built from scratch using C, NASM, and GCC.
> Designed for 386, 486, and Pentium-class CPUs, it runs in protected mode, outputs to VGA text mode and serial ports, and supports floppy/HDD boot with basic FAT support.

---

## ✨ Features

- MBR bootloader at `0x7C00`
- Switch to protected mode with GDT
- A20 gate enabling
- Simple FAT12/FAT16 disk loader stub
- VGA text output (`0xB8000`)
- Serial COM1 support (`0x3F8`)
- Basic kernel (`kmain`) written in C
- Makefile-based build system
- Bootable floppy image for testing in QEMU

---

## ⚙️ Requirements

You’ll need the following tools installed:

- `nasm`
- `gcc` (targeting i386)
- `ld`
- `make`
- `qemu-system-i386`

Optional:

- `gdb`
- `vncviewer` (TigerVNC or similar)

---

## 🛠️ Building ClassicOS

Clone repository:

```sh
git clone https://github.com/gbowne1/ClassicOS.git
cd ClassicOS
```

Run `configure` script to build a cross-compiler toolchain for `i386-elf`:

```sh
./configure
```

Source the `.build.env` file to add the cross-compiler toolchain to your PATH:

```sh
source .build.env
```

Build the kernel:

```sh
make
```
