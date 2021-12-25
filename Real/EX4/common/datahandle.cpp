#include "datahandle.h"
#include "mycommon.h"

#include <stdio.h>

bool DataHandle::load(const char *filename, uint64_t fileOffset, uint64_t len)
{
    FILE *file = fopen(filename, "rb");
    if (file == nullptr)
        return false;

    release();
    m_data = new uint8_t[len];
    fseek(file, fileOffset, SEEK_SET);
    fread(m_data, 1, len, file);
    fclose(file);

    m_len = len;
    m_type = FromFile;
    return true;
}

bool DataHandle::mapTo(uint8_t *ptr, uint64_t len)
{
    release();

    m_data = ptr;
    m_len = len;
    m_type = FromMemory;
    return true;
}

bool DataHandle::mapTo(DataHandle &handle, uint64_t offset, uint64_t len)
{
    if (offset + len > handle.m_len)
        return false;

    release();
    m_data = handle.m_data + offset;
    m_len = len;
    m_type = FromMemory;
    return true;
}

bool DataHandle::save(const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if(file == nullptr)
        return false;
    fwrite(m_data, 1, m_len, file);
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