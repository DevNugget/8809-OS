#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include "../util.h"

#define ATA_PRIMARY_IO      0x1F0
#define ATA_PRIMARY_CTRL    0x3F6
#define ATA_MASTER          0xE0

#define ATA_REG_DATA        0x00
#define ATA_REG_ERROR       0x01
#define ATA_REG_SECCOUNT0   0x02
#define ATA_REG_LBA0        0x03
#define ATA_REG_LBA1        0x04
#define ATA_REG_LBA2        0x05
#define ATA_REG_HDDEVSEL    0x06
#define ATA_REG_COMMAND     0x07
#define ATA_REG_STATUS      0x07

#define ATA_CMD_READ_PIO    0x20
#define ATA_CMD_WRITE_PIO   0x30
#define ATA_CMD_CACHE_FLUSH 0xE7
#define ATA_SR_BSY          0x80
#define ATA_SR_DRQ          0x08
#define ATA_SR_ERR 0x01  // Indicates an error occurred

static void ata_wait();

void ata_read_sector(uint32_t lba, uint8_t* buf);
void ata_write_sector(uint32_t lba, const uint8_t* buf);

#endif
