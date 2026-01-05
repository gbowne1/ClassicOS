#include <stdint.h>

// ATA IO Ports
#define ATA_PRIMARY_DATA            0x1F0
#define ATA_PRIMARY_ERR_FEATURES    0x1F1
#define ATA_PRIMARY_SEC_COUNT       0x1F2
#define ATA_PRIMARY_LBA_LOW         0x1F3
#define ATA_PRIMARY_LBA_MID         0x1F4
#define ATA_PRIMARY_LBA_HIGH        0x1F5
#define ATA_PRIMARY_DRIVE_SEL       0x1F6
#define ATA_PRIMARY_COMM_STAT       0x1F7

// ATA Commands
#define ATA_CMD_READ_PIO            0x20
#define ATA_CMD_WRITE_PIO           0x30

// ELF Ident indexes
#define EI_NIDENT                   16

// Program header types
#define PT_NULL                     0
#define PT_LOAD                     1

// Disk sector size
#define SECTOR_SIZE                 512

// Kernel start LBA
#define KERN_START_SECT             5

extern uint8_t read_buf[];

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

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm__ volatile ("inw %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

static inline void ata_wait_bsy() {
    while (inb(ATA_PRIMARY_COMM_STAT) & 0x80);
}

static inline void ata_wait_drq() {
    while (!(inb(ATA_PRIMARY_COMM_STAT) & 0x08));
}

static void ata_read_sector(void *addr, uint32_t lba) {
    ata_wait_bsy();

    outb(ATA_PRIMARY_DRIVE_SEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_SEC_COUNT, 1);
    outb(ATA_PRIMARY_LBA_LOW, (uint8_t)lba);
    outb(ATA_PRIMARY_LBA_MID, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_COMM_STAT, ATA_CMD_READ_PIO);

    uint16_t* ptr = (uint16_t*)addr;
    ata_wait_bsy();
    ata_wait_drq();
    for (int i = 0; i < 256; i++) {
        *ptr++ = inw(ATA_PRIMARY_DATA);
    }
}

static void load_segment(uint8_t *addr, uint32_t offset, uint32_t size)
{
    uint32_t lba = KERN_START_SECT + offset / SECTOR_SIZE;
    uint32_t off = offset % 512;
    uint8_t data[512];

    while (size > 0) {
        ata_read_sector(data, lba);

        uint32_t copy = 512 - off;
        if (copy > size) {
            copy = size;
        }

        for (uint32_t i = 0; i < copy; i++) {
            addr[i] = data[off + i];
        }

        addr += copy;
        size -= copy;
        lba++;
        off = 0;
    }
}

// Load an ELF executable into memory.
static int elf_load(const void *data) {
    const Elf32_Ehdr* header = (const Elf32_Ehdr*)data;
    const Elf32_Phdr* ph = (const Elf32_Phdr*)((uint8_t*)data + header->e_phoff);

    for (int i = 0; i < header->e_phnum; i++) {
        if (ph[i].p_type != PT_LOAD)
            continue;

        uint32_t offset = ph[i].p_offset;
        uint32_t vaddr = ph[i].p_vaddr;
        uint32_t filesz = ph[i].p_filesz;
        uint32_t memsz = ph[i].p_memsz;

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

static uint32_t
total_headers_size(const Elf32_Ehdr *header) {
    uint32_t phend = header->e_phoff + header->e_phentsize*header->e_phnum;

    // Align to sector size
    uint32_t a = SECTOR_SIZE-1;
    return (phend + a) & ~a;
}

void *load_kernel(void) {
    // Read the first sector
    ata_read_sector(read_buf, KERN_START_SECT);

    const Elf32_Ehdr* header = (const Elf32_Ehdr*)read_buf;

    // Remaining data size, subtract the first 512B already read
    uint32_t rem = total_headers_size(header) - SECTOR_SIZE;

    // Read the rest if necessary
    if (rem) {
        uint8_t *dst = read_buf + SECTOR_SIZE;
        for (uint32_t i = 0; i < rem / SECTOR_SIZE; i++, dst += 512) {
            ata_read_sector(dst, KERN_START_SECT + i + 1);
        }
    }

    elf_load(read_buf);

    return (void *)header->e_entry;
}
