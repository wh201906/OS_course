#include "mydisk.h"
#include "myutil.h"
#include <stdio.h>

MyPartition MyDisk::partition(uint8_t id)
{
    MyPartition result;
    MBR_t mbr;
    DPT_item dpt;
    mbr = MBR();
    if (!mbr.isValid())
        return result;

    dpt = mbr.DPT(id);
    if (!dpt.isValid())
        return result;

    result.mapTo(m_data + *dpt.startSec() * BPS, *dpt.secSize() * BPS);
    return result;
}

void MyDisk::info()
{
    DataHandle::info();
    printf("MyDisk\n");
    MBR().info();
}

bool MyDisk::init()
{
    MBR_t mbr;
    if (!DataHandle::isValid())
        return false;

    mbr = MBR();
    // clean DPT
    for (uint8_t i = 0; i < 4; i++)
        mbr.DPT(i).fill(0x00);
    // set 55 AA
    *(uint16_t *)(mbr.end()) = 0xAA55;

    return true;
}

bool MyDisk::newPartition(uint8_t DPTId, uint64_t startPos, uint64_t len)
{
    // if ((startPos + len) * BPS > m_len)
    //     return false;
    DPT_item dpt = MBR().DPT(DPTId);
    *dpt.isActive() = 0x00; // inactive
    *dpt.FSType() = 0x0B;   // FAT32
    *dpt.startSec() = startPos;
    *dpt.secSize() = len;
    dpt.setStartCHS(LBA2CHS(startPos));
    dpt.setEndCHS(LBA2CHS(startPos + len - 1));
    return true;
}

bool MBR_t::isValid()
{
    return DataHandle::isValid() && *(uint16_t *)end() == 0xAA55;
}

void MBR_t::info()
{
    DataHandle::info();
    printf("MBR_t\n");
    for (uint8_t i = 0; i < 4; i++)
        DPT(i).info();
}

CHS_t DPT_item::getStartCHS()
{
    CHS_t result;
    result.H = *(uint8_t *)(m_data + 1);
    result.S = *(uint8_t *)(m_data + 2) & ((1u << 6) - 1);
    result.C = *(uint8_t *)(m_data + 3) | (uint16_t)(*(uint8_t *)(m_data + 2)) << 2 & 0x300;
    return result;
}

void DPT_item::setStartCHS(CHS_t CHS)
{
    *(m_data + 1) = CHS.H;
    *(m_data + 2) = CHS.S & ((1u << 6) - 1) | CHS.C >> 2 & 0xC0;
    *(m_data + 3) = CHS.C;
}

CHS_t DPT_item::getEndCHS()
{
    CHS_t result;
    result.H = *(uint8_t *)(m_data + 5);
    result.S = *(uint8_t *)(m_data + 6) & ((1u << 6) - 1);
    result.C = *(uint8_t *)(m_data + 7) | (uint16_t)(*(uint8_t *)(m_data + 6)) << 2 & 0x300;
    return result;
}

void DPT_item::setEndCHS(CHS_t CHS)
{
    *(m_data + 5) = CHS.H;
    *(m_data + 6) = CHS.S & ((1u << 6) - 1) | CHS.C >> 2 & 0xC0;
    *(m_data + 7) = CHS.C;
}

bool DPT_item::isValid()
{
    return DataHandle::isValid() && *secSize() > 0;
}

void DPT_item::info()
{
    CHS_t startCHS, endCHS;
    DataHandle::info();
    printf("DPT_item\n");
    if (!isValid())
        return;

    startCHS = getStartCHS();
    endCHS = getEndCHS();
    printf("isActive:0x%02x, FSType:0x%02x, secSize:%u\n", *isActive(), *FSType(), *secSize());
    printf("Start:\n");
    printf("sec:%u, C:%u, H:%u, S:%u\n", *startSec(), startCHS.C, startCHS.H, startCHS.S);
    printf("End:\n");
    printf("sec:%u, C:%u, H:%u, S:%u\n", *startSec() + *secSize() - 1, endCHS.C, endCHS.H, endCHS.S);
}