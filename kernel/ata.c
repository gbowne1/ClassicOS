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

bool ata_wait_ready(void) {
    for (int i = 0; i < ATA_TIMEOUT; i++) {
        uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        /* Must NOT be busy AND must be ready */
        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRDY))
            return true;
    }
    return false;
}

static bool ata_wait(uint8_t mask) {
    for (int i = 0; i < ATA_TIMEOUT; i++) {
        uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        /* If ERR is set, stop waiting and return failure */
        if (status & ATA_SR_ERR) return false;
        
        if (!(status & ATA_SR_BSY) && (status & mask))
            return true;
    }
    return false;
}

bool ata_init(void) {
    /* Select drive */
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER);
    ata_delay();

    /* Check if drive exists */
    uint8_t status = inb(ATA_PRIMARY_IO + ATA_REG_STATUS);
    if (status == 0xFF || status == 0) return false;

    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ata_delay();

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

    /* 1. Wait for drive to be ready for command */
    if (!ata_wait_ready()) return false;

    /* 2. Setup Task File (LBA28) */
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)(lba));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    
    /* 3. Issue Read Command */
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    /* 4. Wait for Data Request (DRQ) */
    if (!ata_wait(ATA_SR_DRQ))
        return false;

    /* 5. Transfer data */
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_PRIMARY_IO);
        buffer[i * 2]     = data & 0xFF;
        buffer[i * 2 + 1] = (data >> 8) & 0xFF;
    }

    ata_delay();
    return true;
}

bool ata_write_sector(uint32_t lba, const uint8_t* buffer) {
    if (!buffer) return false;

    /* 1. Wait for drive to be ready for command */
    if (!ata_wait_ready()) return false;

    /* 2. Setup Task File */
    outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outb(ATA_PRIMARY_IO + ATA_REG_LBA0, (uint8_t)(lba));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA1, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_REG_LBA2, (uint8_t)(lba >> 16));
    
    /* 3. Issue Write Command */
    outb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    /* 4. Wait for drive to request data */
    if (!ata_wait(ATA_SR_DRQ))
        return false;

    /* 5. Transfer data */
    for (int i = 0; i < 256; i++) {
        uint16_t word = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
        outw(ATA_PRIMARY_IO, word);
    }

    ata_delay();
    return true;
}
