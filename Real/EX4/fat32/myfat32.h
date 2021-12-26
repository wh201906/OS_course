#ifndef _MYFAT32_H_
#define _MYFAT32_H_

#include "mycommon.h"
#include "mydisk.h"
#include "dbr.h"
#include "fat.h"

// https://www.win.tue.nl/~aeb/linux/fs/fat/fat-1.html

class MyFAT32 : public MyPartition
{
public:
    // general
    using MyPartition::MyPartition;

    //data
    DBR_t DBR() { return DBR_t(m_data, 512); }
    // FSInfo sector?
    FAT_t FAT(uint8_t id); // id start from 0

    // operations
    bool isValid() override;
    void info() override;
    bool format(uint8_t secPerClust = 1, bool fastMode = true);
};

#endif