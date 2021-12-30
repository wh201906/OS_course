#ifndef _MYFILE_H_
#define _MYFILE_H_

#include "myfat32.h"
#include "dentry.h"
#include "bpb.h"
#include "datacluster.h"
#include <vector>

class MyFile_t
{
public:
    MyFile_t(MyFAT32 &partition);
    bool linkPartition(MyFAT32 &partition);
    uint64_t open(const char *path);
    uint64_t read(uint8_t *buf, uint64_t offset, uint64_t len);
    uint64_t write(uint8_t *buf, uint64_t offset, uint64_t len);
    uint64_t size() { return (m_fileOpened ? fileSize : 0); }
    bool resize(uint64_t newSize);

private:
    bool m_partitionOpened = false;
    bool m_fileOpened = false;
    MyFAT32 &m_partition;
    DataCluster_t m_dataCluster;
    FAT_t m_fat;
    BPB_t m_bpb;
    uint32_t m_rootDirStartID;
    uint32_t m_currFileStartID;
    std::vector<uint32_t> clusterChain;
    uint64_t fileSize = 0;
};

#endif