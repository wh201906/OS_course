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
    uint64_t open(uint8_t *entry);
    uint64_t read(uint8_t *buf, uint64_t offset, uint64_t len);
    uint64_t write(uint8_t *buf, uint64_t offset, uint64_t len);
    uint32_t size() { return (m_fileOpened ? *m_entry.size() : 0); }
    bool resize(uint32_t newSize);

private:
    bool m_partitionOpened = false;
    bool m_fileOpened = false;
    MyFAT32 &m_partition;
    DataCluster_t m_dataCluster;
    FAT_t m_fat;
    BPB_t m_bpb;
    DEntry_t m_entry;
    uint32_t m_rootDirStartID;
    uint32_t m_currFileStartID;
    std::vector<uint32_t> clusterChain;
};

#endif