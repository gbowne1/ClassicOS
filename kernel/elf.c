#include "elf.h"
#include <stddef.h>
#include <string.h>

// This assumes that the elf is ELF32 and little-endian Intel X86 architecture.

// Check if a binary is a valid ELF executable.
int elf_validate(const void* data) {
    const Elf32_Ehdr* header = (const Elf32_Ehdr*)data;

    if (*(uint32_t*)header->e_ident != ELF_MAGIC)
        return 0;

    if (header->e_type != ET_EXEC && header->e_type != ET_DYN)
        return 0;

    if (header->e_machine != EM_386)
        return 0;

    return 1;
}

// Load an ELF executable into memory.
int elf_load(const void* data, void (*load_segment)(uint32_t vaddr, const void* src, uint32_t size)) {
    const Elf32_Ehdr* header = (const Elf32_Ehdr*)data;
    const Elf32_Phdr* ph = (const Elf32_Phdr*)((uint8_t*)data + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD)
            continue;

        const void* src = (uint8_t*)data + ph[i].p_offset;
        uint32_t vaddr = ph[i].p_vaddr;
        uint32_t filesz = ph[i].p_filesz;
        uint32_t memsz = ph[i].p_memsz;

        // Copy data segment
        load_segment(vaddr, src, filesz);

        // Zero remaining BSS (if any)
        if (memsz > filesz) {
            uint8_t* bss_start = (uint8_t*)(vaddr + filesz);
            for (uint32_t j = 0; j < memsz - filesz; j++) {
                bss_start[j] = 0;
            }
        }
    }

    return header->e_entry;
}
