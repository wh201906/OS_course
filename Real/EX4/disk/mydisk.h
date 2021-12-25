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
// sizeof(MBR_t) = 512;
struct MBR_t
{
    uint8_t bootCode[446];
    uint8_t DPTData[4][16];
    uint8_t end[2] = {0x55, 0xAA};
};

// get DPT items from MBR
void Disk_getDPT(const MBR_t *MBR, DPT_item *item, uint8_t id);
void Disk_setDPT(MBR_t *MBR, const DPT_item *item, uint8_t id);

#endif