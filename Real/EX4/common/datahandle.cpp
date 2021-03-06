#include "datahandle.h"
#include "mycommon.h"
#include "myutil.h"

bool DataHandle::load(const char *filename, uint64_t srcOffset, uint64_t len)
{
    FILE *file = fopen(filename, "rb");
    if (file == nullptr)
        return false;

    release();
    m_data = new uint8_t[len];
    fseek(file, srcOffset, SEEK_SET);
    fread(m_data, 1, len, file);
    fclose(file);

    m_len = len;
    m_type = FromFile;
    afterDataChanged();
    return true;
}

bool DataHandle::mapTo(uint8_t *ptr, uint64_t len)
{
    release();

    m_data = ptr;
    m_len = len;
    m_type = FromMemory;
    afterDataChanged();
    return true;
}

bool DataHandle::mapTo(DataHandle &handle, uint64_t srcOffset, uint64_t len)
{
    if (srcOffset + len > handle.m_len)
        return false;

    release();
    m_data = handle.m_data + srcOffset;
    m_len = len;
    m_type = FromMemory;
    afterDataChanged();
    return true;
}

bool DataHandle::save(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == nullptr)
        return false;
    fwrite(m_data, 1, m_len, file);
    fclose(file);
    return true;
}

bool DataHandle::save(const char *filename, uint64_t srcOffset, uint64_t dstOffset, uint64_t len)
{
    FILE *file;
    if (srcOffset + len > m_len)
        return false;
    file = fopen(filename, "r+b");
    if (file == nullptr)
        return false;
    fseek(file, dstOffset, SEEK_SET);
    fwrite(m_data + srcOffset, 1, len, file);
    fclose(file);
    return true;
}

void DataHandle::release()
{
    if (m_type == FromFile && m_data != nullptr)
        delete m_data;
    m_data = nullptr;
    m_len = 0;
}

bool DataHandle::isValid()
{
    return m_data != nullptr;
}

void DataHandle::info()
{
    printf("--------------------------\n");
    printf("data:0x%p, len:%llu", m_data, m_len);
    for (uint16_t factor = 512; factor <= 4096; factor *= 2)
    {
        if (m_len / factor > 1 && m_len % factor == 0)
            printf("=%u*%u", m_len / factor, factor);
        else
            break;
    }
    putchar('\n');
    printf("type:%s, valid:%c\n", m_type == FromFile ? "FromFile" : "FromMemory", isValid() ? 'T' : 'F');
}

void DataHandle::showHex(uint64_t len)
{
    if (!DataHandle::isValid())
    {
        printf("showHex(): invalid!\n");
    }
    else
    {
        printHex(m_data, len);
    }
}

bool DataHandle::fill(uint8_t data)
{
    if (!DataHandle::isValid())
        return false;
    for (uint64_t i = 0; i < m_len; i++)
        m_data[i] = data;
    return true;
}
