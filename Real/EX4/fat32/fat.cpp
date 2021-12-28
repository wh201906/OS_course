#include "fat.h"

void FAT_t::info()
{
    DataHandle::info();
    printf("FAT_t\n");
}

bool FAT_t::init(uint64_t bps, uint64_t rootDirId, uint8_t mediaType)
{
    if (!DataHandle::isValid())
        return false;

    fill(0x00);
    *item(0) = Item0Prefix | mediaType;
    *item(1) = ItemEnd;
    *item(rootDirId) = ItemEnd;
    return true;
}

uint64_t FAT_t::nextFree(uint64_t start)
{
    uint32_t *ptr = (uint32_t *)m_data;
    for (ptr += start; (uint8_t *)ptr - m_data < m_len; ptr++)
    {
        if (*ptr == ItemFree)
            break;
    }
    if ((uint8_t *)ptr - m_data >= m_len)
        return 0;
    else
        return (ptr - (uint32_t *)m_data);
}

uint64_t FAT_t::freeNum()
{
    uint64_t n, f;
    n = num();
    f = 0;
    for (uint64_t i = 0; i < n; i++)
    {
        if (*item(i) == ItemFree)
            f++;
    }
    return f;
}