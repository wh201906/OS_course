#ifndef _FSINFO_H_
#define _FSINFO_H_

#include "datahandle.h"
#include "bpb.h"
#include "fat.h"

class FSInfo_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint32_t *sig1() { return (uint32_t *)(m_data + 0); } // len: 4 // 0x41615252
    // uint8_t reserved[480]                              // len: 480
    uint32_t *sig2() { return (uint32_t *)(m_data + 484); }           // len: 4 // 0x61417272
    uint32_t *freeClustNum() { return (uint32_t *)(m_data + 488); }   // len: 4
    uint32_t *nextFreeClust() { return (uint32_t *)(m_data + 492); } // len: 4
    // uint8_t reserved[12]                                // len: 480
    uint32_t *end() { return (uint32_t *)(m_data + 508); } // len: 4 // 0xAA550000

    // operations
    bool isValid() override;
    void info() override;
    bool init(BPB_t &bpb, FAT_t &fat);
};

#endif