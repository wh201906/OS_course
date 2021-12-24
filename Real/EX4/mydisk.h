#ifndef _MYDISK_H_
#define _MYDISK_H_

#include "mycommon.h"

struct DPT_item
{
    uint8_t isActive;
    uint8_t FSType;
    CHS_t startCHS, endCHS;
    uint32_t startSec, secSize;
};

struct MBR_t
{
    uint8_t bootCode[446];
    uint8_t DPT[4][16];
    uint16_t end; // 0x55 0xAA->0xAA55(little endian)
};

void Disk_getDPT(uint8_t (*data)[16], DPT_item items[4]);

#endif