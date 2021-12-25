#ifndef _MYFAT32_H_
#define _MYFAT32_H_

#include "mycommon.h"
#include "mydisk.h"

class BPB_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint16_t *bytesPerSec() { return (uint16_t *)(m_data + 0); }    // len: 2
    uint8_t *secPerClust() { return (m_data + 2); }                 // len: 1
    uint16_t *reservedSecNum() { return (uint16_t *)(m_data + 3); } // len: 2
    uint8_t *FATNum() { return (m_data + 5); }                      // len: 1
    // uint16_t rootDirNum;                                         // len: 2
    // uint16_t secNum_small;                                       // len: 2
    uint8_t *mediaType() { return (m_data + 10); } // len: 1
    // uint16_t secPerFAT;                         // len: 2
    uint16_t *secPerTrack() { return (uint16_t *)(m_data + 13); }    // len: 2
    uint16_t *headNum() { return (uint16_t *)(m_data + 15); }        // len: 2
    uint32_t *hiddenSecNum() { return (uint32_t *)(m_data + 17); }   // len: 4 // equals to DPT_item->startSec
    uint32_t *secNum() { return (uint32_t *)(m_data + 21); }         // len: 4
    uint32_t *FATSize() { return (uint32_t *)(m_data + 25); }        // len: 4 // in sectors
    uint16_t *FATflag() { return (uint16_t *)(m_data + 29); }        // len: 2
    uint16_t *FSVer() { return (uint16_t *)(m_data + 31); }          // len: 2
    uint32_t *rootDirClustId() { return (uint32_t *)(m_data + 33); } // len: 4
    uint16_t *FSISecId() { return (uint16_t *)(m_data + 37); }       // len: 2
    uint16_t *DBRBakSecId() { return (uint16_t *)(m_data + 39); }    // len: 2
    // uint8_t reserved[12];                                         // len: 12
    uint8_t *BIOSDrive() { return (m_data + 53); } // len: 1
    // uint8_t reserved;                           // len: 1
    uint8_t *EDPBSig() { return (m_data + 55); }             // len: 1
    uint32_t *serial() { return (uint32_t *)(m_data + 56); } // len: 4
    uint8_t *label() { return (m_data + 60); }               // len: 11
    uint8_t *FSLabel() { return (m_data + 71); }             // len: 8

    // operations
    bool isValid() override;
    void info() override;
};

class DBR_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint8_t *jmp() { return (m_data + 0); }        // len: 3
    uint8_t *OEM() { return (m_data + 3); }        // len: 8
    BPB_t BPB() { return BPB_t(m_data + 11, 90); } // len: 79
    uint8_t *bootCode() { return (m_data + 90); }  // len: 420
    uint8_t *end() { return (m_data + 510); }      // len: 2

    // operations
    bool isValid() override;
    void info() override;
};

class MyFAT32 : public MyPartition
{
public:
    // general
    using MyPartition::MyPartition;

    //data
    DBR_t DBR() { return DBR_t(m_data, 512); }

    // operations
    bool isValid() override;
    void info() override;
};

#endif