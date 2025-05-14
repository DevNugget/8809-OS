#include "ff.h"         /* Obtains integer types */
#include "diskio.h"
#include "../vga_driver.h"
#include <stdint.h>
#include "ata.h"
#include "../string.h"

DSTATUS disk_initialize(BYTE pdrv) {
    return 0; // Assume always ready for now
}

DSTATUS disk_status(BYTE pdrv) {
    return 0;
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    for (UINT i = 0; i < count; i++) {
        ata_read_sector((uint32_t)(sector + i), buff + (i * 512));
    }
    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count) {
    terminal_writestring("Sector: ");
    terminal_hexprint(sector);
    terminal_writestring("\nCount: ");
    terminal_hexprint(count);
    terminal_writestring("\n");

    for (UINT i = 0; i < count; i++) {
        ata_write_sector((uint32_t)(sector + i), buff + (i * 512));
    }


    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    return RES_OK; // Stubbed
}
