#include "mydisk.h"

void Disk_getDPT(const MBR_t *MBR, DPT_item *item, uint8_t id)
{
    uint8_t *ptr = (uint8_t *)(MBR->DPTData[id]);
    item->isActive = *(ptr + 0);
    item->startCHS.H = *(uint8_t *)(ptr + 1);
    item->startCHS.S = *(uint8_t *)(ptr + 2) & ((1u << 6) - 1);
    item->startCHS.C = *(uint8_t *)(ptr + 3) | (uint16_t)(*(uint8_t *)(ptr + 2)) << 2 & 0x300;
    item->FSType = *(ptr + 4);
    item->endCHS.H = *(uint8_t *)(ptr + 5);
    item->endCHS.S = *(uint8_t *)(ptr + 6) & ((1u << 6) - 1);
    item->endCHS.C = *(uint8_t *)(ptr + 7) | (uint16_t)(*(uint8_t *)(ptr + 6)) << 2 & 0x300;
    item->startSec = *(uint32_t *)(ptr + 8);
    item->secSize = *(uint32_t *)(ptr + 12);
}

void Disk_setDPT(MBR_t *MBR, const DPT_item *item, uint8_t id)
{
    uint8_t *ptr = (uint8_t *)(MBR->DPTData[id]);
    *(ptr + 0) = item->isActive;
    *(ptr + 1) = item->startCHS.H;
    *(ptr + 2) = item->startCHS.S & ((1u << 6) - 1) | item->startCHS.C >> 2 & 0xC0;
    *(ptr + 3) = item->startCHS.C;
    *(ptr + 4) = item->FSType;
    *(ptr + 5) = item->endCHS.H;
    *(ptr + 6) = item->endCHS.S & ((1u << 6) - 1) | item->endCHS.C >> 2 & 0xC0;
    *(ptr + 7) = item->endCHS.C;
    *(uint32_t *)(ptr + 8) = item->startSec;
    *(uint32_t *)(ptr + 12) = item->secSize;
}