#ifndef _MYDIR_H_
#define _MYDIR_H_

#include "myfat32.h"
#include "dentry.h"
#include "bpb.h"
#include "datacluster.h"
#include <vector>
#include <string>

class MyDir_t
{
public:
    MyDir_t(MyFAT32 &partition);
    bool open(MyFAT32 &partition);
    uint64_t ls();
    bool cd(char *path);
    uint32_t pwd(char *result); // return the length of path, '\0' excluded.
    // find DEntry_t by name in current directory
    // return the address of DEntry_t, or nullptr if not found
    uint8_t *find(const char *name, uint32_t DirID, uint8_t attr = DEntry_t::Directory | DEntry_t::Archive);

private:
    bool m_opened = false;
    MyFAT32 &m_partition;
    DataCluster_t m_dataCluster;
    FAT_t m_fat;
    BPB_t m_bpb;
    uint32_t m_rootDirStartID;
    uint32_t m_currDirStartID;
    std::vector<std::string> currPath;
};

#endif