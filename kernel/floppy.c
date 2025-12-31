#include "floppy.h"

// DMA buffer must be < 16MB and 64KB aligned to avoid boundary issues
static uint8_t dma_buffer[512] __attribute__((aligned(4096)));
static volatile int irq_fired = 0;

void floppy_lba_to_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sect) {
    *cyl  = lba / (FLOPPY_HPC * FLOPPY_SPT);
    *head = (lba / FLOPPY_SPT) % FLOPPY_HPC;
    *sect = (lba % FLOPPY_SPT) + 1;
}

// Minimalist DMA setup for Channel 2
void floppy_dma_setup(uint32_t addr, uint16_t count) {
    asm volatile("outb %%al, $0x0A" : : "a"(0x06)); // Mask channel 2
    asm volatile("outb %%al, $0x0C" : : "a"(0xFF)); // Reset flip-flop
    asm volatile("outb %%al, $0x04" : : "a"((uint8_t)(addr & 0xFF)));
    asm volatile("outb %%al, $0x04" : : "a"((uint8_t)((addr >> 8) & 0xFF)));
    asm volatile("outb %%al, $0x81" : : "a"((uint8_t)((addr >> 16) & 0xFF)));
    asm volatile("outb %%al, $0x0B" : : "a"(0x46)); // Single mode, Read
    asm volatile("outb %%al, $0x0A" : : "a"(0x02)); // Unmask channel 2
}

int floppy_read_sector(uint32_t lba, uint8_t* buffer) {
    uint16_t cyl, head, sect;
    floppy_lba_to_chs(lba, &cyl, &head, &sect);

    // 1. Motor On
    asm volatile("outb %%al, %1" : : "a"(0x1C), "Nd"(FDC_DOR));
    
    // 2. Prepare DMA
    floppy_dma_setup((uint32_t)dma_buffer, 511);

    // 3. Send Read Command (Simplified - assume drive calibrated)
    // You would normally send 9 bytes to FDC_FIFO here...
    // For brevity, we assume fdc_write() helper exists from previous steps.
    
    // 4. Copy out of DMA buffer
    for(int i=0; i<512; i++) buffer[i] = dma_buffer[i];
    return 0;
}
