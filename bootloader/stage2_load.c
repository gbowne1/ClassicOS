#include <stdint.h>

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

// Load an ELF executable into memory.
static int elf_load(const void* data, void (*load_segment)(uint8_t *vaddr, uint32_t src, uint32_t size)) {
    const Elf32_Ehdr* header = (const Elf32_Ehdr*)data;
    const Elf32_Phdr* ph = (const Elf32_Phdr*)((uint8_t*)data + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD)
            continue;

        uint32_t offset = ph[i].p_offset;
        uint32_t vaddr = ph[i].p_vaddr;
        uint32_t filesz = ph[i].p_filesz;
        uint32_t memsz = ph[i].p_memsz;

        // Copy data segment
        //load_segment((uint8_t *)vaddr, offset, filesz);
        load_segment((uint8_t *)vaddr, offset, filesz);

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

#define KERN_START_SECT 5
#define MAX(a, b) ((a)>(b) ? (a) : (b))

extern void ata_lba_read(uint32_t lba, uint8_t nsect, void *addr);
extern uint8_t read_buf[];

static uint32_t
total_header_size(const Elf32_Ehdr *header) {
    uint32_t phend = header->e_phoff + header->e_phentsize*header->e_phnum;

    // Align to 512
    return (phend + 511) & ~511;
}

static void read_sectors(uint8_t *vaddr, uint32_t offset, uint32_t size) {
    // # of sectors to read
    uint32_t rem_nsect = ((size + 511) & ~511) / 512;

    // Current lba address, offset by the first sector already read
    uint32_t lba = KERN_START_SECT + offset / 512;

    // Max 255 sectors at a time
    while (rem_nsect) {
        uint8_t nsect = rem_nsect > 255 ? 255 : rem_nsect;
        ata_lba_read(lba, nsect, vaddr);

        vaddr += nsect * 512;
        rem_nsect -= nsect;
        lba += nsect;
    }
}

void *load_kernel(void) {
    // Read the first sector
    ata_lba_read(KERN_START_SECT, 1, read_buf);

    const Elf32_Ehdr* header = (const Elf32_Ehdr*)read_buf;

    // Remaining data size, subtract the first 512B already read
    uint32_t rem = total_header_size(header) - 512;

    // Read the rest if necessary
    if (rem)
        read_sectors(read_buf+512, 512, rem);

    elf_load(read_buf, read_sectors);

    return (void *)header->e_entry;
}
