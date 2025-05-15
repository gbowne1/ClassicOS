#ifndef ELF_H
#define ELF_H

#include <stdint.h>

#define ELF_MAGIC 0x464C457F  // "\x7FELF" in little-endian

// ELF Types
#define ET_EXEC 2
#define ET_DYN  3

// ELF Machine
#define EM_386  3

// ELF Ident indexes
#define EI_NIDENT 16

// Program header types
#define PT_NULL    0
#define PT_LOAD    1

// ELF Header (32-bit)
typedef struct {
    uint8_t  e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;     // Entry point
    uint32_t e_phoff;     // Program header table offset
    uint32_t e_shoff;     // Section header table offset
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} __attribute__((packed)) Elf32_Ehdr;

// Program Header (32-bit)
typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} __attribute__((packed)) Elf32_Phdr;

#endif // ELF_H
