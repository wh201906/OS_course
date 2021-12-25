#ifndef _DATAHANDLE_H_
#define _DATAHANDLE_H_

#include "mycommon.h"

class DataHandle
{
public:
    enum DataType
    {
        FromFile = 0,
        FromMemory
    };

    bool load(const char *filename, uint64_t fileOffset, uint64_t m_len);
    bool mapTo(uint8_t *ptr, uint64_t len);
    bool mapTo(DataHandle& handle, uint64_t offset, uint64_t len);
    bool save(const char *filename);
    void release();
    bool isValid();

    uint8_t *data() { return m_data; } // unsafe
    uint64_t len() { return m_len; }
    DataType type() { return m_type; }

private:
    uint8_t *m_data = nullptr;
    uint64_t m_len = 0;
    DataType m_type = FromFile;
};

#endif