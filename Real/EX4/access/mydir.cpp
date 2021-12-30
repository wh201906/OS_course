#include "mydir.h"
#include "myutil.h"
#include <queue>

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

bool MyDir_t::cd(const char *path)
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

    // split path by '/', then try to access every part and update newDirStartID
    std::vector<std::string> vec = split(path, "/");
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

uint8_t *MyDir_t::findHelper(uint32_t DirID, std::function<bool(DEntry_t &)> cond, bool checkValidity)
{
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
            if (checkValidity && !entry.isValid())
                continue;
            if (cond(entry))
            {
                found = true;
                break;
            }
        }
        if (!found)
            DirID = *m_fat.item(DirID);
        else
            break;
    }
    if (!found)
        return nullptr;
    else
        return entry.data();
}

uint8_t *MyDir_t::find(const char *name, uint32_t DirID, uint8_t attr)
{
    return findHelper(DirID,
                      [&](DEntry_t &entry) -> bool {
                          char currName[13];
                          entry.fullName(currName);
                          return (strcmp(currName, name) == 0 && *entry.attribute() & attr);
                      });
}

uint8_t *MyDir_t::findFree(uint32_t DirID)
{
    return findHelper(
        DirID,
        [&](DEntry_t &entry) -> bool {
            return entry.DataHandle::isValid() && !entry.isValid();
        },
        false);
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

bool MyDir_t::mkHelper(const char *name, std::function<void(DEntry_t &, uint32_t)> proc)
{
    if (!m_opened)
        return false;
    if (strlen(name) > 8)
        return false;
    if (find(name))
        return false;
    uint8_t *pos;
    pos = findFree(m_currDirStartID);
    // create first, allocate new cluster if directory become full after creating
    // if pos = nullptr there, the partition might be full(failed to get new cluster in last allocation)
    if (pos == nullptr)
    {
        // unexpected
        return false;
    }
    DEntry_t newEntry(pos, 32);
    uint32_t id = m_fat.nextFree();
    if (id == 0)
    {
        // unexpected
        return false;
    }
    proc(newEntry, id);
    // allocate new cluster if current directory is full
    if (findFree(m_currDirStartID) == nullptr)
    {
        uint16_t end;
        id = m_fat.nextFree();
        if (id == 0)
            return true; // new directory created, but failed to allocate new cluster for current directory(full now)
        end = m_currDirStartID;
        while (m_fat.isItemValid(*m_fat.item(end)))
            end = *m_fat.item(end);
        *m_fat.item(end) = id;
        *m_fat.item(id) = FAT_t::ItemEnd;
        // clean up new cluster
        pos = m_dataCluster.cluster(id);
        for (uint64_t i = 0; i < m_bpb.bytesPerClust(); i += 32)
            *(pos + i) = 0x00;
    }
    return true;
}

bool MyDir_t::mkdir(const char *name)
{
    return mkHelper(
        name,
        [&](DEntry_t &entry, uint32_t id) {
            // DEntry in current directory
            entry.init(DEntry_t::Directory);
            memcpy(entry.name(), name, strlen(name));
            entry.setClusterId(id);
            // FAT Item
            *m_fat.item(id) = FAT_t::ItemEnd;
            // DEntry in new directory
            uint8_t *pos = m_dataCluster.cluster(id);
            DEntry_t newEntry(pos, 32);
            newEntry.init(DEntry_t::Directory);
            newEntry.name()[0] = '.';
            newEntry.setClusterId(id);
            newEntry = DEntry_t(pos + 32, 32);
            newEntry.init(DEntry_t::Directory);
            memcpy(newEntry.name(), "..", 2);
            newEntry.setClusterId(m_currDirStartID);
            // clean up new cluster
            // set the first byte of every 32 bytes to 0x00
            // so DEntry_t::isValid() will return false for unused DEntry
            for (uint64_t i = 64; i < m_bpb.bytesPerClust(); i += 32)
                *(pos + i) = 0x00;
        });
}

bool MyDir_t::mkfile(const char *name, uint32_t size)
{
    return mkHelper(
        name,
        [&](DEntry_t &entry, uint32_t id) {
            // DEntry in current directory
            entry.init(DEntry_t::Archive);
            memcpy(entry.name(), name, strlen(name));
            entry.setClusterId(id);
            *entry.size() = size;
            // FAT Item
            *m_fat.item(id) = FAT_t::ItemEnd;
        });
}

bool MyDir_t::rename(const char *oldName, const char *newName)
{
    if (strlen(oldName) > 12)
        return false;

    uint8_t *pos = find(oldName);
    if (pos == nullptr)
        return false;
    DEntry_t entry(pos, 32);
    return entry.rename(newName);
}

bool MyDir_t::remove(const char *name)
{
    if (!m_opened)
        return false;
    DEntry_t entry(find(name), 32);
    if (!entry.isValid())
        return false;
    if (*entry.attribute() & DEntry_t::Directory)
    {
        char n[20];
        std::queue<uint8_t *> dirQueue; // for BFS
        std::vector<uint8_t *> dirVec;
        dirQueue.push(entry.data());
        while (!dirQueue.empty())
        {
            DEntry_t currEntry(dirQueue.front(), 32);
            dirVec.push_back(dirQueue.front()); // BFS: access
            dirQueue.pop();
            // BFS: add children(direct neighbourhood)
            uint32_t currID = currEntry.getClusterId();
            uint8_t *d;
            while (m_fat.isItemValid(currID))
            {
                d = m_dataCluster.cluster(currID);
                for (uint64_t i = 0; i < m_bpb.bytesPerClust(); i += 32)
                {
                    DEntry_t childEntry(&d[i], 32);
                    if (!childEntry.isValid())
                        continue;
                    else if (childEntry.name()[0] == '.')
                        continue;
                    else if (!(*childEntry.attribute() & DEntry_t::Directory)) // file, just remove
                        removeOne(childEntry);
                    else // directory, add to queue
                        dirQueue.push(childEntry.data());
                }
                currID = *m_fat.item(currID);
            }
        }
        for (auto it = dirVec.rbegin(); it != dirVec.rend(); it++)
        {
            DEntry_t currEntry(*it, 32);
            removeOne(currEntry);
        }
    }
    else
    {
        removeOne(entry);
    }
    return true;
}

void MyDir_t::removeOne(DEntry_t &entry)
{
    std::vector<uint32_t> idList;
    uint32_t currID = entry.getClusterId();
    idList.push_back(currID);
    while (m_fat.isItemValid(*m_fat.item(currID)))
    {
        currID = *m_fat.item(currID);
        idList.push_back(currID);
    }
    for (auto it = idList.begin(); it != idList.end(); it++)
        *m_fat.item(*it) = FAT_t::ItemFree;
    entry.data()[0] = 0xE5; // removed flag
}
