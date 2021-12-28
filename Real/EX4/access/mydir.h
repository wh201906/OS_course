#ifndef _MYDIR_H_
#define _MYDIR_H_

#include "myfat32.h"
#include "dentry.h"
#include "bpb.h"
#include "datacluster.h"

class MyDir_t
{
public:
    MyDir_t(MyFAT32 &partition);
    bool open(MyFAT32 &partition);
    uint64_t ls();
    bool cd(char *path);

private:
    bool m_opened = false;
    MyFAT32 &m_partition;
    uint32_t m_rootDirStartID;
    uint32_t m_currDirStartID;
};

#endif