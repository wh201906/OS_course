#include "mydir.h"
#include <string>
#include <regex>

MyDir_t::MyDir_t(MyFAT32 &partition) : m_partition(partition)
{
    open(partition);
}

bool MyDir_t::open(MyFAT32 &partition)
{
    BPB_t bpb;
    if (!partition.isValid())
        return false;
    bpb = m_partition.DBR().BPB();
    if (!bpb.isValid())
        return false;
    m_partition = partition;
    m_rootDirStartID = *bpb.rootDirClustId();
    m_currDirStartID = m_rootDirStartID;
    m_opened = true;
    return true;
}

uint64_t MyDir_t::ls()
{
    char filename[13];
    char fileext[4];
    if (!m_opened)
        return 0;
    DataCluster_t data = m_partition.DataCluster();
    FAT_t fat = m_partition.FAT(0);
    BPB_t bpb = m_partition.DBR().BPB();
    uint64_t result = 0;
    uint32_t currID = m_currDirStartID;
    uint8_t *d;
    while (fat.isItemValid(currID))
    {
        d = data.cluster(currID);

        for (uint64_t i = 0; i < bpb.bytesPerClust(); i += 32)
        {
            DEntry_t entry(&d[i], 32);
            if (!entry.isValid())
                continue;
            entry.fullName(filename);
            printf("%s\n", filename);
            result++;
        }
        currID = *fat.item(currID);
    }
    return result;
}

bool MyDir_t::cd(char *path)
{
    DataCluster_t data = m_partition.DataCluster();
    FAT_t fat = m_partition.FAT(0);
    BPB_t bpb = m_partition.DBR().BPB();
    uint32_t newDirStartID = m_currDirStartID;
    char filename[13];

    // absolute path
    if (path[0] == '/')
        newDirStartID = m_rootDirStartID;

    std::string pathStr(path);
    std::regex re("/");

    // split path by '/', then try to access every part and update newDirStartID
    // use regex unmatched(-1 for 4th arg) result for splitting, ignore empty parts(5th arg)
    std::vector<std::string> vec(std::sregex_token_iterator(pathStr.begin(), pathStr.end(), re, -1, std::regex_constants::match_not_null), std::sregex_token_iterator());
    for (auto part = vec.begin(); part != vec.end(); part++)
    {
        bool found = false;
        uint32_t currID = newDirStartID;
        // loop for directorys use 2 or more FAT items
        while (fat.isItemValid(currID) && !found)
        {
            // root directory doesn't have ".." and ".", handle it
            if (newDirStartID == m_rootDirStartID && (part->compare(".") == 0 || part->compare("..") == 0))
            {
                found = true;
                // newDirStartID = m_rootDirStartID;
                break;
            }
            // search
            uint8_t *d = data.cluster(currID);
            for (uint64_t i = 0; i < bpb.bytesPerClust(); i += 32)
            {
                DEntry_t entry(&d[i], 32);
                if (!entry.isValid())
                    continue;
                entry.fullName(filename);
                if (part->compare(filename) == 0 && *entry.attribute() & DEntry_t::Directory)
                {
                    found = true;
                    newDirStartID = entry.getClusterId();
                    // special case
                    // "/*/"
                    if (strcmp("..", filename) == 0 && newDirStartID == 0)
                        newDirStartID = m_rootDirStartID;
                    break;
                }
            }
            if (!found)
            {
                currID = *fat.item(currID);
            }
            else
                break;
        }
        if (!found)
            return false;
    }
    m_currDirStartID = newDirStartID;
    return true;
}