#include <stdint.h>
#include "util.h"
#include "ata.h"

static void ata_wait() {
    while (inportb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY);
    while (!(inportb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_DRQ));
}

void ata_read_sector(uint32_t lba, uint8_t* buf) {
    outportb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER | ((lba >> 24) & 0x0F));
    outportb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA0, lba & 0xFF);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA1, (lba >> 8) & 0xFF);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA2, (lba >> 16) & 0xFF);
    outportb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait();

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buf)[i] = inportw(ATA_PRIMARY_IO + ATA_REG_DATA);
    }
}