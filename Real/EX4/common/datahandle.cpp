#include "datahandle.h"
#include "mycommon.h"

#include <stdio.h>

DataHandle::~DataHandle()
{
    release();
}

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
    printf("data:0x%p, len:%llu\n", m_data, m_len);
    printf("type:%s, valid: %c\n", m_type == FromFile ? "FromFile" : "FromMemory", isValid() ? 'T' : 'F');
}

void DataHandle::showHex(uint64_t len)
{
    if(!isValid())
    {
        printf("showHex(): invalid!\n");
    }
    else
    {
        for (uint64_t i = 0; i < len; i++)
        {
            printf("%02x ", *(m_data+i));
            if(i % 16 == 15)
                printf("\n");
            else if(i % 8 == 7)
                printf("  ");
        }
    }
}

void DataHandle::showHex()
{
    showHex(m_len);
}