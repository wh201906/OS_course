#ifndef _MYFAT32_H_
#define _MYFAT32_H_

#include "mycommon.h"
#include "mydisk.h"
#include "dbr.h"
#include "fat.h"
#include "fsinfo.h"
#include "datacluster.h"

// https://www.win.tue.nl/~aeb/linux/fs/fat/fat-1.html

class MyFAT32 : public MyPartition
{
public:
    // general
    using MyPartition::MyPartition;

    //data
    DBR_t DBR() { return DBR_t(m_data, BPS); }
    FSInfo_t FSInfo() { return FSInfo_t(m_data + BPS, BPS); } // the backup FSInfo sector is not used.
    FAT_t FAT(uint8_t id);                                    // id start from 0
    DataCluster_t DataCluster();

    // operations
    bool isValid() override;
    void info() override;
    bool format(uint8_t secPerClust = 1, bool fastMode = true);
    bool syncFAT(uint8_t src, uint8_t dst);
    bool syncFAT() { return syncFAT(0, 1); }
};

#endif