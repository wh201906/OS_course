#include "myfile.h"
#include "mydir.h"

MyFile_t::MyFile_t(MyFAT32 &partition) : m_partition(partition)
{
    linkPartition(partition);
}

bool MyFile_t::linkPartition(MyFAT32 &partition)
{
    m_partition = partition;
    if (!partition.isValid())
        return false;
    m_bpb = m_partition.DBR().BPB();
    if (!m_bpb.isValid())
        return false;
    m_dataCluster = m_partition.DataCluster();
    m_fat = m_partition.FAT(0);
    m_rootDirStartID = *m_bpb.rootDirClustId();
    m_partitionOpened = true;
    return true;
}

bool MyFile_t::open(const char *path)
{
    if (!m_partitionOpened)
        return false;
    size_t tmp;
    tmp = strlen(path);
    if (tmp == 0 || path[tmp - 1] == '/')
        return false;
    MyDir_t dir(m_partition);
    std::string pathStr(path);
    size_t slashPos = pathStr.rfind('/');
    std::string nameStr = pathStr.substr(slashPos + 1);
    pathStr.erase(slashPos + 1);
    if (!dir.cd(pathStr.c_str()))
        return false;
    uint8_t *entryPtr = dir.find(nameStr.c_str());
    if (entryPtr == nullptr)
        return false;
    clusterChain.clear();
    DEntry_t entry(entryPtr, 32);
    fileSize = *entry.size();
    uint32_t currID = entry.getClusterId();
    clusterChain.push_back(currID);
    while (m_fat.isItemValid(*m_fat.item(currID)))
    {
        currID = *m_fat.item(currID);
        clusterChain.push_back(currID);
    }
    return true;
}