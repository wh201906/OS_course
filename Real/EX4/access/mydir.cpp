#include "mydir.h"
#include <regex>
#include <stack>

MyDir_t::MyDir_t(MyFAT32 &partition) : m_partition(partition)
{
    open(partition);
}

bool MyDir_t::open(MyFAT32 &partition)
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

    uint64_t result = 0;
    uint32_t currID = m_currDirStartID;
    uint8_t *d;
    while (m_fat.isItemValid(currID))
    {
        d = m_dataCluster.cluster(currID);

        for (uint64_t i = 0; i < m_bpb.bytesPerClust(); i += 32)
        {
            DEntry_t entry(&d[i], 32);
            if (!entry.isValid())
                continue;
            entry.fullName(filename);
            printf("%s\n", filename);
            result++;
        }
        currID = *m_fat.item(currID);
    }
    return result;
}

bool MyDir_t::cd(char *path)
{
    if (!m_opened)
        return false;
    uint32_t newDirStartID = m_currDirStartID;
    std::vector<std::string> newPath;
    // absolute path
    if (path[0] == '/')
    {
        newDirStartID = m_rootDirStartID;
    }
    else
    {
        newPath = currPath;
    }

    std::string pathStr(path);
    std::regex re("/");

    // split path by '/', then try to access every part and update newDirStartID
    // use regex unmatched(-1 for 4th arg) result for splitting
    std::vector<std::string> vec(std::sregex_token_iterator(pathStr.begin(), pathStr.end(), re, -1), std::sregex_token_iterator());
    for (auto part = vec.begin(); part != vec.end(); part++)
    {
        // ignore empty parts
        if (part->empty())
            continue;
        // root directory doesn't have ".." and ".", handle it
        if (newDirStartID == m_rootDirStartID && (part->compare(".") == 0 || part->compare("..") == 0))
            continue;
        DEntry_t entry(find(part->c_str(), newDirStartID, DEntry_t::Directory), 32);
        if (!entry.isValid())
            return false;
        newDirStartID = entry.getClusterId();
        // special case after found
        // "/*/"
        if (part->compare("..") == 0 && newDirStartID == 0)
            newDirStartID = m_rootDirStartID;
        if (part->compare("..") == 0)
        {
            newPath.pop_back();
        }
        else if (part->compare(".") != 0)
        {
            newPath.push_back(*part);
        }
    }
    m_currDirStartID = newDirStartID;
    currPath = newPath;
    return true;
}

uint8_t *MyDir_t::find(const char *name, uint32_t DirID, uint8_t attr)
{
    char currName[13];
    bool found = false;
    DEntry_t entry;
    // loop for directorys using 2 or more FAT items
    while (m_fat.isItemValid(DirID) && !found)
    {
        // search
        uint8_t *d = m_dataCluster.cluster(DirID);
        for (uint64_t i = 0; i < m_bpb.bytesPerClust(); i += 32)
        {
            entry = DEntry_t(&d[i], 32);
            if (!entry.isValid())
                continue;
            entry.fullName(currName);
            if (strcmp(currName, name) == 0 && *entry.attribute() & attr)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            DirID = *m_fat.item(DirID);
        }
        else
            break;
    }
    if (!found)
        return nullptr;
    else
        return entry.data();
}

uint32_t MyDir_t::pwd(char *result)
{
    uint32_t i = 0;
    result[i++] = '/';
    for (auto it = currPath.cbegin(); it != currPath.cend(); it++)
    {
        strcpy(result + i, it->c_str());
        i += it->length();
        result[i++] = '/';
    }
    result[i] = '\0';
    return i;
}