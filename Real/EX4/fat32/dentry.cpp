#include "dentry.h"

void DEntry_t::info()
{
    DataHandle::info();
    printf("DEntry_t\n");
}

bool DEntry_t::isValid()
{
    return DataHandle::isValid() && m_data[0] != '\x00' && m_data[0] != ' ' && m_data[0] != '\xE5' && (!!(*attribute() & Directory)) ^ (*size() != 0);
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

void DEntry_t::fullName(char *result)
{
    uint8_t i, j;
    if (!isValid())
    {
        result[0] = '\0';
        return;
    }
    memcpy(result, name(), 8);
    for (i = 0; i < 8; i++)
    {
        if (result[i] == ' ')
            break;
    }

    if (*extension() == ' ') // no extension
    {
        result[i] = '\0';
        return;
    }
    result[i] = '.';
    result[++i] = '\0';
    memcpy(result + i, extension(), 3);
    for (j = 0; j < 3; j++)
    {
        if (result[i + j] == ' ')
            break;
    }
    result[i + j] = '\0';
}