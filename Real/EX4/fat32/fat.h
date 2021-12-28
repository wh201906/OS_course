#ifndef _FAT_H_
#define _FAT_H_

#include "datahandle.h"

class FAT_t : public DataHandle
{
public:
    enum ItemData : uint32_t
    {
        ItemFree = 0x00000000,
        Item0Prefix = 0x0FFFFF00,
        ItemBad = 0x0FFFFFF7,
        ItemEnd = 0x0FFFFFFF,
    };
    // general
    using DataHandle::DataHandle;

    //data
    uint32_t *item(uint64_t id) { return ((uint32_t *)m_data + id); } // the first 2 item is calculated

    // operations
    void info() override;
    bool init(uint64_t bps = BPS, uint64_t rootDirId = 2, uint8_t mediaType = 0xF8);
    uint64_t nextFree(uint64_t start = 2);         // return 0 if no free items are found
    uint64_t num() { return (m_len / 4); } // the first 2 item is calculated
    uint64_t freeNum();
    bool isItemValid(uint32_t item) { return (item > ItemFree && item < ItemBad); }
};

#endif