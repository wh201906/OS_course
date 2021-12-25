#include "mydisk.h"
#include <stdio.h>

MBR_t MyDisk::MBR()
{
    MBR_t mbr;
    mbr.mapTo(m_data, 512);
    return mbr;
}

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

    result.mapTo(m_data + *dpt.startSec() * 512, *dpt.secSize() * 512);
    return result;
}

void MyDisk::info()
{
    DataHandle::info();
    MBR().info();
}

DPT_item MBR_t::DPT(uint8_t id)
{
    DPT_item dptItem;
    dptItem.mapTo(m_data + 446 + 16 * id, 16);
    return dptItem;
}

bool MBR_t::isValid()
{
    return DataHandle::isValid() && *(uint16_t *)end() == 0xAA55;
}

void MBR_t::info()
{
    DataHandle::info();
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