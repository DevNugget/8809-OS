#include <stdint.h>
#include "../util.h"
#include "../vga_driver.h"
#include "ata.h"

static void ata_wait() {
    uint8_t status;

    // Wait for BSY to clear
    do {
        status = inportb(ATA_PRIMARY_IO + ATA_REG_STATUS);
    } while (status & ATA_SR_BSY);

    //terminal_writestring("BSY cleared.\n");

    // Now wait for DRQ
    int timeout = 100000;
    while (!(status & ATA_SR_DRQ) && timeout--) {
        status = inportb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        if (status & ATA_SR_ERR) {
            gterminal_writestring("ATA error during wait!\n");
            break;
        }
    }
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

void ata_write_sector(uint32_t lba, const uint8_t* buf) {
    // 1. Wait until drive is not busy
    while (inportb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY);

    // 2. Select drive and LBA mode
    outportb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, ATA_MASTER | ((lba >> 24) & 0x0F));

    // 3. Send sector count and LBA
    outportb(ATA_PRIMARY_IO + ATA_REG_SECCOUNT0, 1);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA0, lba & 0xFF);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA1, (lba >> 8) & 0xFF);
    outportb(ATA_PRIMARY_IO + ATA_REG_LBA2, (lba >> 16) & 0xFF);

    // 4. Send write command
    outportb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    // 5. Wait for DRQ or ERR
    uint8_t status;
    do {
        status = inportb(ATA_PRIMARY_IO + ATA_REG_STATUS);
        if (status & ATA_SR_ERR) {
            terminal_writestring("ATA error: can't write sector\n");
            return;
        }
    } while (!(status & ATA_SR_DRQ));

    // 6. Write 256 words (512 bytes)
    for (int i = 0; i < 256; i++) {
        outportw(ATA_PRIMARY_IO + ATA_REG_DATA, ((uint16_t*)buf)[i]);
    }

    // 7. Flush (optional for now)
    outportb(ATA_PRIMARY_IO + ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);
    while (inportb(ATA_PRIMARY_IO + ATA_REG_STATUS) & ATA_SR_BSY);
}
