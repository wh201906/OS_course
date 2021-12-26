#ifndef _FAT_H_
#define _FAT_H_

#include "datahandle.h"

class FAT_t : public DataHandle
{
public:
    enum itemData : uint32_t
    {
        ItemEmpty = 0x00000000,
        Item0Prefix = 0x0FFFFF00,
        ItemBad = 0x0FFFFFF7,
        ItemEnd = 0x0FFFFFFF,
    };
    // general
    using DataHandle::DataHandle;

    //data
    uint32_t *item(uint64_t id) { return ((uint32_t *)m_data + id); }

    // operations
    void info() override;
    bool init(uint8_t mediaType = 0xF8);
    uint64_t nextEmpty(uint64_t start = 2); // return 0 if no empty items are found
};

#endif