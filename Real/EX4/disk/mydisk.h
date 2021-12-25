#ifndef _MYDISK_H_
#define _MYDISK_H_

#include "mycommon.h"
#include "datahandle.h"

typedef DataHandle MyPartition;

class DPT_item : public DataHandle
{
public:
    // data
    uint8_t *isActive() { return (m_data + 0); }
    CHS_t getStartCHS();
    void setStartCHS(CHS_t CHS);
    uint8_t *FSType() { return (m_data + 4); }
    CHS_t getEndCHS();
    void setEndCHS(CHS_t CHS);
    uint32_t *startSec() { return (uint32_t *)(m_data + 8); }
    uint32_t *secSize() { return (uint32_t *)(m_data + 12); }

    // operations
    bool isValid() override;
    void info() override;
};

class MBR_t : public DataHandle
{
public:
    // data
    uint8_t *bootCode() { return (m_data + 0); } // len: 446
    DPT_item DPT(uint8_t id);                    // len: 16 * 4 = 64
    uint8_t *end() { return (m_data + 510); }    // len: 2

    // operations
    bool isValid() override;
    void info() override;
};

class MyDisk : public DataHandle
{
public:
    // data
    MBR_t MBR();
    MyPartition partition(uint8_t id);

    // operations
    void info() override;
};

#endif