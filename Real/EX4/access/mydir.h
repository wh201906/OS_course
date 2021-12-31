#ifndef _MYDIR_H_
#define _MYDIR_H_

#include "myfat32.h"
#include "dentry.h"
#include "bpb.h"
#include "datacluster.h"
#include <vector>
#include <string>
#include <functional>

class MyDir_t
{
public:
    MyDir_t(MyFAT32 &partition);
    bool open(MyFAT32 &partition);
    uint64_t ls();
    bool cd(const char *path);
    uint32_t pwd(char *result); // return the length of path, '\0' excluded.
    // find DEntry_t by name in current directory
    // return the address of DEntry_t, or nullptr if not found
    uint8_t *find(const char *name, uint32_t DirID, uint8_t attr = DEntry_t::Directory | DEntry_t::Archive);
    uint8_t *find(const char *name, uint8_t attr = DEntry_t::Directory | DEntry_t::Archive) { return find(name, m_currDirStartID, attr); }
    // find next free DEntry
    uint8_t *findFree(uint32_t DirID);
    bool mkdir(const char *name);
    bool mkfile(const char *name, uint32_t size);
    bool rename(const char *oldName, const char *newName); // in-place rename, because this FAT32 implementation only supports short filename
    bool remove(const char *name); // remove file/directory in the current directory by name
    bool move(const char *oldPath, const char *newPath); // newPath doesn't contain the new name.
    uint64_t tryShrink();
    uint32_t currDirStartID() { return m_currDirStartID; }

private:
    bool m_opened = false;
    MyFAT32 &m_partition;
    DataCluster_t m_dataCluster;
    FAT_t m_fat;
    BPB_t m_bpb;
    uint32_t m_rootDirStartID;
    uint32_t m_currDirStartID;
    std::vector<std::string> currPath;

    // helper functions
    uint8_t *findHelper(uint32_t DirID, std::function<bool(DEntry_t &)> cond, bool checkValidity = true);
    bool mkHelper(const char *name, std::function<void(DEntry_t &, uint32_t)> proc);
    void removeOne(DEntry_t &entry);
};

#endif