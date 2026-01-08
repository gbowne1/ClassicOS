#include <stddef.h>
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
#define PH_PER_SECTOR               (SECTOR_SIZE / sizeof(Elf32_Phdr))

// Kernel start LBA
#define KERN_START_SECT             5

// VGA
// Expects bios initialization for text mode (3), buffer at 0xb8000
#define VGA_ADDRESS                 0xB8000
#define VGA_COLS                    80
#define VGA_ROWS                    25

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

static void ata_read_sectors(uint8_t *addr, uint32_t offset, uint32_t size)
{
    // Reads are offset from the starting sector of the kernel
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

static void on_error(const char *msg)
{
    uint16_t *ptr = (uint16_t *)VGA_ADDRESS;

    // Clear
    uint16_t val = 0x0f00 | (uint8_t)' ';
    for (size_t i = 0; i < VGA_COLS * VGA_ROWS; i++) {
        ptr[i] = val;
    }

    // Print error
    for (size_t i = 0; msg[i]; i++) {
        ptr[i] = 0xf00 | (uint8_t)msg[i];
    }

    // Halt
    while (1) {
        __asm__("hlt");
    }
}

// Load an ELF executable into memory.
// NOTE: Only 32-byte program headers are supported.
// Returns the entry point to the program.
static void *elf_load(const void *data) {
    const Elf32_Ehdr *header = (const Elf32_Ehdr*)data;

    if (header->e_phentsize != sizeof(Elf32_Phdr)) {
        // The bootloader only handles 32-byte program header entries
        on_error("ERROR: Unsupported program header entry size, halting...");
    }

    // Buffer for the program headers
    uint8_t file_buf[SECTOR_SIZE];

    // Current file offset to the next program header
    uint32_t file_offset = header->e_phoff;

    for (int i = 0; i < header->e_phnum; i++) {
        // Check for sector boundary.
        // Program headers are read in a sector at a time
        // 512 / 32 = 16 PH per sector
        if (i % PH_PER_SECTOR == 0) {
            uint32_t count = (header->e_phnum - i) * sizeof(Elf32_Phdr);
            if (count > SECTOR_SIZE) {
                count = SECTOR_SIZE;
            }

            // Reads
            ata_read_sectors(file_buf, file_offset, count);
            file_offset += count;
        }

        // PH being processed currently, index mod 16 as headers
        // are being loaded in sector by sector.
        const Elf32_Phdr *ph = (const Elf32_Phdr *)file_buf + (i % PH_PER_SECTOR);

        // Discard non-load segments
        if (ph->p_type != PT_LOAD)
            continue;

        // Load in the segment
        uint32_t offset = ph->p_offset;
        uint32_t filesz = ph->p_filesz;
        uint32_t memsz = ph->p_memsz;
        uint8_t *vaddr = (uint8_t *)ph->p_vaddr;
        ata_read_sectors(vaddr, offset, filesz);

        // Zero remaining BSS (if any)
        if (memsz > filesz) {
            uint8_t* bss_start = vaddr + filesz;
            for (uint32_t j = 0; j < memsz - filesz; j++) {
                bss_start[j] = 0;
            }
        }
    }

    // Return the entry point
    return (void *)header->e_entry;
}

void *load_kernel(void) {
    // ELF header buffer
    uint8_t header_buf[SECTOR_SIZE];

    // Read the first sector (contains the ELF header)
    ata_read_sector(header_buf, KERN_START_SECT);

    // `elf_load()` returns the entry point
    return elf_load(header_buf);
}
