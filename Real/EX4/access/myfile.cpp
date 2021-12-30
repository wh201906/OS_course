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
    m_fileOpened = false;
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
    m_fileOpened = true;
    return true;
}

uint64_t MyFile_t::read(uint8_t *buf, uint64_t offset, uint64_t len)
{
    // data(head is always aligned):
    // |-cluster-|...|-cluster-|...|-cluster-|
    // |-cluster-|...|-cluster-|...|----|
    // read:
    // |-----offset-----|---required----|
    if (!m_fileOpened)
        return false;
    uint64_t currLen;
    uint32_t currIdx;
    uint8_t *data;
    // get actural valid length
    currLen = fileSize - offset;
    currLen = currLen < len ? currLen : len;
    len = currLen;
    // find first clust by offset
    currIdx = offset / m_bpb.bytesPerClust();

    uint64_t headOffset = offset % m_bpb.bytesPerClust();
    if (headOffset != 0) // head not aligned
    {
        uint64_t headLen = m_bpb.bytesPerClust() - headOffset;
        headLen = headLen < currLen ? headLen : currLen;
        data = m_dataCluster.cluster(clusterChain[currIdx++]);
        memcpy(buf, data + headOffset, headLen);
        currLen -= headLen;
        buf += headLen;
    }
    while (currLen > m_bpb.bytesPerClust()) // complete cluster
    {
        data = m_dataCluster.cluster(clusterChain[currIdx++]);
        memcpy(buf, data, m_bpb.bytesPerClust());
        currLen -= m_bpb.bytesPerClust();
        buf += m_bpb.bytesPerClust();
    }
    if (currLen > 0) // tail not aligned
    {
        data = m_dataCluster.cluster(clusterChain[currIdx]);
        memcpy(buf, data, currLen);
    }
    return len;
}