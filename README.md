# ClassicOS

An x86 Operating System for 386, 486, Pentium class (P-75, 100, Pentium II, P3, P4, etc) CPU's

## Tooling

These are the versions I use, but please use the latest possible versions.

- NASM version 2.14
- QEMU x86_64
- GNU ld (GNU Binutils for Debian) 2.31.1 or newer
- dd (coreutils) 8.30
- as (GNU Binutils for Debian) 2.31.1 or newer
- ld (GNU Binutils for Debian) 2.31.1 or newer
- cc (Debian 8.3.0-6) 8.3.0

The C and C++ standards we are using for this are C17 and C++20

For C/C++:

- clang version 7.0.1-8+deb10u2 (tags/RELEASE_701/final)
- gcc version 8.3.0 (Debian 8.3.0-6)
- g++ version 8.3.0 (Debian 8.3.0-6)
- GNU gdb (Debian 8.2.1-2+b3) 8.2.1
- lldb version 7.0.1
- Coreutils 8.30
- Binutils 2.31.1
- Bison 3.3.2
- Diffutils 3.7
- Findutils 4.6.0.225
- Gawk 4.2.1
- Grep 3.3
- Gzip 1.9
- M4 1.4.18
- Make 4.2.1
- Patch 2.7.6
- Perl 5.28.1
- Python 3.7.3
- Sed 4.7
- Tar 1.30
- Texinfo 6.5, Xz 5.2.4

## Features

- Booting from 1.2M, 1.44MB, hard drive or ISO9660
- FAT12, FAT16, FAT32, NTFS and ext2 support
- GUI
- 