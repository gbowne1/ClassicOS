#ifndef FLOPPY_H
#define FLOPPY_H

#include <stdint.h>

#define FDC_DOR  0x3F2
#define FDC_MSR  0x3F4
#define FDC_FIFO 0x3F5
#define FDC_CCR  0x3F7

// Geometry for 1.44MB floppy
#define FLOPPY_SPT 18
#define FLOPPY_HPC 2

void floppy_init(void);
int  floppy_read_sector(uint32_t lba, uint8_t* buffer);
void floppy_lba_to_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sect);

#endif
