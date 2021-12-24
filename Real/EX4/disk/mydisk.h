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

// mapped to the raw disk data
struct MBR_t
{
    uint8_t bootCode[446];
    uint8_t DPT[4][16];
    uint16_t end = 0xAA55; // 0x55 0xAA->0xAA55(little endian)
};

// get 4 DPT items from MBR
void Disk_getDPT(const MBR_t *MBR, DPT_item items[4]);
// write DPT to MBR
void Disk_writeDPT(MBR_t *MBR, const DPT_item *item, uint8_t id);

#endif