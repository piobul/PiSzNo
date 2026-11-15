#include "ff.h"
#include "diskio.h"
#include "SdCard.hpp"
#include "Timer.hpp"

extern SdCard sdCard; // Obiekt zdefiniowany w main.cpp

extern "C" {

DSTATUS disk_status(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;
    return sdCard.isReady() ? 0 : STA_NOINIT;
}

DSTATUS disk_initialize(BYTE pdrv) {
    if (pdrv != 0) return STA_NOINIT;
    return sdCard.init() ? 0 : STA_NOINIT;
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != 0 || count == 0) return RES_PARERR;
    for (UINT i = 0; i < count; i++) {
        if (!sdCard.readBlock(sector + i, buff + (i * 512))) {
            return RES_ERROR;
        }
    }
    return RES_OK;
}

DRESULT disk_write(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count) {
    if (pdrv != 0 || count == 0) return RES_PARERR;
    for (UINT i = 0; i < count; i++) {
        if (!sdCard.writeBlock(sector + i, buff + (i * 512))) {
            return RES_ERROR;
        }
    }
    return RES_OK;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
    if (pdrv != 0) return RES_PARERR;
    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = 512;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            return RES_OK;
        default:
            return RES_PARERR;
    }
}

// Znacznik czasu modyfikacji pliku w FAT32 (2026-11-15 12:00)
DWORD get_fattime(void) {
    return ((DWORD)(Timer::START_YEAR - 1980) << 25)
         | ((DWORD)Timer::START_MONTH << 21)
         | ((DWORD)Timer::START_DAY << 16)
         | ((DWORD)Timer::START_HOUR << 11)
         | ((DWORD)Timer::START_MIN << 5)
         | ((DWORD)(Timer::START_SEC / 2));
}

} // extern "C"