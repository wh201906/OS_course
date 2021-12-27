#ifndef _DATAHANDLE_H_
#define _DATAHANDLE_H_

#include "mycommon.h"
#include "string.h"

// NOTE:
// For child classes of DataHandle:
//
// Function declarations/definations like
// "<basic type*> funcName() { return ...}"
// will return a pointer of a parameter, the value can be change by it.
//
// Function declarations like
// "<class name> funcName();"
// will return a object of DataHandle, which shares part of the m_data.
//
// Function declarations like
// "<type> getXXX();" and "void setXXX(<type> arg);"
// are the setter/getter of some values, which are not directly storaged in the memory/file.
//
// WARNING:
// The DataHandle might be mapped to an invalid memory area.
class DataHandle
{
public:
    enum DataType
    {
        FromFile = 0, // owns a buffer
        FromMemory    // mapped to memory area
    };

    explicit DataHandle(){}
    explicit DataHandle(const char *filename, uint64_t srcOffset, uint64_t len) { load(filename, srcOffset, len); }
    explicit DataHandle(uint8_t *ptr, uint64_t len) { mapTo(ptr, len); }
    explicit DataHandle(DataHandle &handle, uint64_t srcOffset, uint64_t len) { mapTo(handle, srcOffset, len); }
    ~DataHandle() { release(); }

    bool load(const char *filename, uint64_t srcOffset, uint64_t len);
    bool mapTo(uint8_t *ptr, uint64_t len);
    bool mapTo(DataHandle &handle, uint64_t srcOffset, uint64_t len);
    bool save(const char *filename);                                                       // save, overwrite the whole file
    bool save(const char *filename, uint64_t srcOffset, uint64_t dstOffset, uint64_t len); // save, overwrite from the dstOffset
    void release();
    virtual bool isValid();
    virtual void info();

    uint8_t *data() { return m_data; } // unsafe
    uint64_t len() { return m_len; }
    DataType type() { return m_type; }

    void showHex(uint64_t len);
    void showHex() { showHex(m_len); }
    virtual bool fill(uint8_t data);

protected:
    virtual void afterDataChanged() {}

    uint8_t *m_data = nullptr;
    uint64_t m_len = 0;
    DataType m_type = FromFile;
};

#endif