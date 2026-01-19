#include "ata.h"
#include "io.h"
#include "print.h"

#define ATA_TIMEOUT 100000

static inline void ata_delay(void) {
    /* 400ns delay by reading alternate status */
    inb(ATA_PRIMARY_CTRL);
    inb(ATA_PRIMARY_CTRL);
    inb(ATA_PRIMARY_CTRL);
    inb(ATA_PRIMARY_CTRL);
}

static bool ata_wait(uint8_t mask) {
    for (int i = 0; i < ATA_TIMEOUT; i++) {
        uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        if (!(status & ATA_SR_BSY) && (status & mask))
            return true;
    }
    return false;
}

bool ata_init(void) {
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0); // master
    ata_delay();

    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ata_delay();

    uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
    if (status == 0)
        return false; // no drive

    if (!ata_wait(ATA_SR_DRQ))
        return false;

    uint16_t identify[256];
    for (int i = 0; i < 256; i++)
        identify[i] = inw(ATA_PRIMARY_IO);

    print_string("[ATA] Primary master detected\n");
    return true;
}

bool ata_read_sector(uint32_t lba, uint8_t* buffer) {
    if (!buffer) return false;

    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)(lba));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    if (!ata_wait(ATA_SR_DRQ))
        return false;

    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_PRIMARY_IO);
        buffer[i * 2]     = data & 0xFF;
        buffer[i * 2 + 1] = data >> 8;
    }

    ata_delay();
    return true;
}

bool ata_write_sector(uint32_t lba, const uint8_t* buffer) {
    if (!buffer) return false;

    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)(lba));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    if (!ata_wait(ATA_SR_DRQ))
        return false;

    for (int i = 0; i < 256; i++) {
        uint16_t word =
            buffer[i * 2] |
            (buffer[i * 2 + 1] << 8);
        outw(ATA_PRIMARY_IO, word);
    }

    ata_delay();
    return true;
}
