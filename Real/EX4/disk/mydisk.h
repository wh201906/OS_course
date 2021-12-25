#ifndef _MYDISK_H_
#define _MYDISK_H_

#include "mycommon.h"
#include "datahandle.h"

typedef DataHandle MyPartition;

class DPT_item : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    // data
    uint8_t *isActive() { return (m_data + 0); } // len: 1
    CHS_t getStartCHS();                         // len: 3
    void setStartCHS(CHS_t CHS);
    uint8_t *FSType() { return (m_data + 4); } // len: 1
    CHS_t getEndCHS();                         // len: 3
    void setEndCHS(CHS_t CHS);
    uint32_t *startSec() { return (uint32_t *)(m_data + 8); } // len: 4
    uint32_t *secSize() { return (uint32_t *)(m_data + 12); } // len: 4

    // operations
    bool isValid() override;
    void info() override;
};

class MBR_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    // data
    uint8_t *bootCode() { return (m_data + 0); }                              // len: 446
    DPT_item DPT(uint8_t id) { return DPT_item(m_data + 446 + 16 * id, 16); } // len: 16 * 4 = 64
    uint8_t *end() { return (m_data + 510); }                                 // len: 2

    // operations
    bool isValid() override;
    void info() override;
};

class MyDisk : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    // data
    MBR_t MBR() { return MBR_t(m_data, 512); } // len: 512
    MyPartition partition(uint8_t id);

    // operations
    void info() override;
};

#endif