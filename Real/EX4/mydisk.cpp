#include "mydisk.h"

void Disk_getDPT(uint8_t (*data)[16], DPT_item items[4])
{
    uint8_t *ptr = (uint8_t *)data;
    for (int i = 0; i < 4; i++)
    {
        ptr += i * 16;
        items[i].isActive = *ptr;
        items[i].startCHS.H = *(uint8_t *)(ptr + 1);
        items[i].startCHS.S = *(uint8_t *)(ptr + 2) & ((1u << 6) - 1);
        items[i].startCHS.C = *(uint8_t *)(ptr + 3) | (uint16_t)(*(uint8_t *)(ptr + 2)) << 2 & 0x300;
        items[i].FSType = *(ptr + 4);
        items[i].endCHS.H = *(uint8_t *)(ptr + 5);
        items[i].endCHS.S = *(uint8_t *)(ptr + 6) & ((1u << 6) - 1);
        items[i].endCHS.C = *(uint8_t *)(ptr + 7) | (uint16_t)(*(uint8_t *)(ptr + 6)) << 2 & 0x300;
        items[i].startSec = *(uint32_t *)(ptr + 8);
        items[i].secSize = *(uint32_t *)(ptr + 12);
    }
}