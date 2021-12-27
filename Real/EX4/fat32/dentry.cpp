#include "dentry.h"

void DEntry_t::info()
{
    DataHandle::info();
    printf("DEntry_t\n");
}

bool DEntry_t::init(uint8_t attr)
{
    if (!DataHandle::isValid())
        return false;

    fill(0x00);
    memcpy(name(), justSpaces, 8);
    memcpy(extension(), justSpaces, 3);
    *attribute() = attr;
    return true;
}

uint32_t DEntry_t::getClusterId()
{
    uint16_t lo;
    uint32_t hi;
    lo = *(uint16_t *)(m_data + 26);
    hi = *(uint16_t *)(m_data + 20);
    hi <<= 16;
    hi |= lo;
    return hi;
}

void DEntry_t::setClusterId(uint32_t id)
{
    *(uint16_t *)(m_data + 26) = id & 0x0000FFFF;
    *(uint16_t *)(m_data + 20) = id >> 16 & 0x0000FFFF;
}