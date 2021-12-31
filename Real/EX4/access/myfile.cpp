#include "myfile.h"
#include "mydir.h"
#include "math.h"

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

uint64_t MyFile_t::open(const char *path)
{
    if (!m_partitionOpened)
        return 0;
    size_t tmp;
    tmp = strlen(path);
    if (tmp == 0 || path[tmp - 1] == '/')
        return 0;
    MyDir_t dir(m_partition);
    std::string pathStr(path);
    size_t slashPos = pathStr.rfind('/');
    std::string nameStr = pathStr.substr(slashPos + 1);
    pathStr.erase(slashPos + 1);
    if (!dir.cd(pathStr.c_str()))
        return 0;
    uint8_t *entryPtr = dir.find(nameStr.c_str());
    if (entryPtr == nullptr)
        return 0;
    clusterChain.clear();
    m_entry = DEntry_t(entryPtr, 32);
    uint32_t currID = m_entry.getClusterId();
    clusterChain.push_back(currID);
    while (m_fat.isItemValid(*m_fat.item(currID)))
    {
        currID = *m_fat.item(currID);
        clusterChain.push_back(currID);
    }
    m_fileOpened = true;
    return *m_entry.size();
}

uint64_t MyFile_t::open(uint8_t *entry)
{
    clusterChain.clear();
    m_entry = DEntry_t(entry, 32);
    uint32_t currID = m_entry.getClusterId();
    clusterChain.push_back(currID);
    while (m_fat.isItemValid(*m_fat.item(currID)))
    {
        currID = *m_fat.item(currID);
        clusterChain.push_back(currID);
    }
    m_fileOpened = true;
    return *m_entry.size();
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
    currLen = *m_entry.size() - offset;
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

uint64_t MyFile_t::write(uint8_t *buf, uint64_t offset, uint64_t len)
{
    if (!m_fileOpened)
        return false;
    uint64_t currLen;
    uint32_t currIdx;
    uint8_t *data;
    // get actural valid length
    currLen = *m_entry.size() - offset;
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
        memcpy(data + headOffset, buf, headLen);
        currLen -= headLen;
        buf += headLen;
    }
    while (currLen > m_bpb.bytesPerClust()) // complete cluster
    {
        data = m_dataCluster.cluster(clusterChain[currIdx++]);
        memcpy(data, buf, m_bpb.bytesPerClust());
        currLen -= m_bpb.bytesPerClust();
        buf += m_bpb.bytesPerClust();
    }
    if (currLen > 0) // tail not aligned
    {
        data = m_dataCluster.cluster(clusterChain[currIdx]);
        memcpy(data, buf, currLen);
    }
    return len;
}

bool MyFile_t::resize(uint32_t newSize)
{
    if (newSize == *m_entry.size())
        return true;
    else if (newSize < *m_entry.size()) // shrink
    {
        uint64_t newClusterNum = ceil((double)newSize / m_bpb.bytesPerClust());
        if (newClusterNum == 0)
            newClusterNum = 1; // allocate at least one cluster even the size is 0
        while (clusterChain.size() > newClusterNum)
        {
            *m_fat.item(clusterChain.back()) = FAT_t::ItemFree;
            clusterChain.pop_back();
            *m_fat.item(clusterChain.back()) = FAT_t::ItemEnd;
        }
    }
    else // enlarge
    {
        uint64_t newClusterNum = ceil((double)newSize / m_bpb.bytesPerClust());
        if (newClusterNum == 0)
            newClusterNum = 1; // allocate at least one cluster even the size is 0
        if (m_fat.freeNum() < newClusterNum - clusterChain.size())
            return false;
        while (clusterChain.size() < newClusterNum)
        {
            uint32_t id;
            uint8_t *pos;
            id = m_fat.nextFree();
            *m_fat.item(clusterChain.back()) = id;
            *m_fat.item(id) = FAT_t::ItemEnd;
            pos = m_dataCluster.cluster(id);
            for (uint64_t i = 0; i < m_bpb.bytesPerClust(); i++) // clear new cluster
                *(pos + i) = 0x00;
            clusterChain.push_back(id);
        }
    }
    *m_entry.size() = newSize;
    return true;
}