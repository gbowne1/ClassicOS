# ClassicOS

This operating system uses standard operating system concepts used in the 32 bit environment. It will eventually be ported to 64 bit including IA64.
This ClassicOS operating system, aims to support major hardware and software technology existing from when the first 32 bit systems appeared on the market through the early 2000's and most of those have been listed below.

## Programming

This project uses the C library and Assembly language.

## Toolchain

GNU Make 4.2.1
CMake 3.13.4
GNU 8.3.0
gcc (Debian 8.3.0-6) 8.3.0
nasm 2.14
GNU ld (GNU Binutils for Debian) 2.31.1
binutils

For testing, QEMU i386 and TigerVNC/VNCViewer on ::1:5900

## Development (Team, etc)

This project will use MIT or the GPL license and will be fully open source.

Ideal situations aside, My goal has always been:

1-2 person working on bootloader
1-2 person working on kernel
1-2 person working on drivers
1-2 person working on issues/bugs
1-2 people working on applications/user-space
1-2 people working on Networking
1-2 people working on memmory issues, stack issues, etc.

at least one person doing hardware and software testing and writing tests in a test framework

At least one major bug fix a week
At least one minor buf fix a week
At least one new feature a month (or more)

## CPU Processor Support

This project initially aims to support all 32 bit Intel and AMD processors Including;

-- Intel --
i386 SX and DX Processors
i486 SX and DX Processors
Pentium Processors (60 to 120MHz)
Pentium Pro Processors
Pentium II Processors
Pentium II Xeon Processors
Pentium III Processors
Pentium III Xeon Processors
early Pentium 4 Processors (Willamette, Prescott, Northwood)

-- AMD --

AMD k5
AMD K6
AMD am386
AMD am486
Athlon
Duron
Sempron

## Device Support

USB 1.0
USB 1.1
USB 2.0
USB 2.1
SATA 1
UDMA
UltraATA 66/100/133
SCSI-1
SCSI-2
SCSI-3
Ultra-2 SCSI
Ultra-3 SCSI
ESDI
MFM/RLL

## Booting

BIOS from

- primary hard disk partition
- primary floppy
- ISO 9660 CD-ROM, CD-R,CD-RW,
- DVD
- Removable media (Zip, Jaz, USB, Tape, Syquest, Bernoulli, CF, SmartMedia, SD etc)

Might eventually support GRUB/GRUB2 and/or UEFI/EFI.

Include support for AHCI and ACPI

## Bus Support

ISA
EISA
VESA/VESA Local Bus (VLB)
PCI
PCI-X
PCIe 1.0, 1.1, 2.0
AGP
DIN 41416/NuBUS

## Hardware support

This OS aims to support major hardware existing from 1985 to early 2k's.

## Memory Support

up to 4GB

## Features

Has a GUI
Has a IDE
Has a text editor
Has compilers for compiled lanugages (C, C++, C#, Go, Java, Fortran, Pascal, Objective C, Haskell, ADA, Scala, Rust, Zig, Ocaml, Julia, Dart, Erlang, Elixir)
Has a Web Browser
Has a shell, tty, console, terminal
Has interpreters for interpreted languages like python 2 and python 3, JavaScript, BASIC, PHP, etc.
Has a git client
Has a video, audio editing and playing suite
Has a file browser (in GUI)
Has debuggers for languages/compilers, etc that output symbols, etc.

## Video Support

-- Modes --
CGA
EGA
VGA
SVGA
MCGA
XGA
HGA / Hercules
XGA-2
SXGA
UXGA
WXGA
8514/a
VESA SVGA
VESA/VLB
AGP (1.0, 2.0, 3.0, 3.5, Pro) - 66MHz - aka AGP 1X, 2X, 4X, 8X
PCI Graphics

-- Resolutions --

## Networking

Novell NE1000
Novell NE2000

## Drivers

-- Video card(s)
-- NIC's (3Com, Intel, etc.)
-- Audio

## Filesystems

- FAT12
- FAT16/FAT16B/FAT16X
- FAT32/FAT32X
- NTFS
- HPFS
- HFS / HFS+
- ext / ext2 / ext3
- exFAT
- ZFS
- JFS
