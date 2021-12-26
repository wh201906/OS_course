#include "myfat32.h"
#include "string.h"
#include <stdio.h>

FAT_t MyFAT32::FAT(uint8_t id)
{
    BPB_t bpb;
    if (!isValid())
        return FAT_t();

    bpb = DBR().BPB();
    if (!bpb.isValid())
        return FAT_t();

    return FAT_t(m_data + (*bpb.reservedSecNum() + id * *bpb.FATSize()) * *bpb.bytesPerSec(), *bpb.FATSize());
}

bool MyFAT32::isValid()
{
    return MyPartition::isValid() && DBR().isValid();
}

void MyFAT32::info()
{
    DBR_t dbr = DBR();
    BPB_t bpb = dbr.BPB();
    MyPartition::info();
    printf("MyFAT32\n");
    dbr.info();
    for (uint8_t i = 0; i < *bpb.FATNum(); i++)
    {
        FAT(i).info();
    }
}

bool MyFAT32::format(uint8_t secPerClust, bool fastMode)
{
    DBR_t dbr;
    BPB_t bpb;
    if (!MyPartition::isValid())
        return false;
    if (secPerClust == 0 || secPerClust & (secPerClust - 1) != 0)
        return false;
    if ((uint32_t)secPerClust * BPS > 65536)
        return false;

    dbr = DBR();
    bpb = dbr.BPB();
    // set DBR
    memcpy(dbr.jmp(), "\xEB\x58\x90", 3);
    memcpy(dbr.OEM(), "MSDOS5.0", 8);
    *(uint16_t *)(dbr.end()) = 0xAA55;
    // set BPB
    // 1. FATSize * BPS / 4 == DataSize / SPC + 2
    // 2. FATNum * FATSize + DataSize == secNum
    // 1, 2 => FATSize_actural == floor((4secNum + 8SPC) / (4FATNum + BPS*SPC))
    // unused: secNum - ((4FATNum + BPS*SPC)FATSize_actural - 8SPC) / 4
    bpb.fill(0x00);
    *bpb.bytesPerSec() = BPS;
    *bpb.secPerClust() = secPerClust;
    *bpb.reservedSecNum() = 32;
    *bpb.FATNum() = 2;
    *bpb.mediaType() = 0xF8;
    *bpb.secPerTrack() = SPT;
    *bpb.headNum() = HPC;
    *bpb.hiddenSecNum() = 63; // might be different if 4k aligned
    *bpb.secNum() = m_len / *bpb.bytesPerSec();
    *bpb.FATSize() = (4 * *bpb.secNum() + 8 * *bpb.secPerClust()) / (4 * *bpb.FATNum() + *bpb.bytesPerSec() * *bpb.secPerClust());
    *bpb.FATflag() = 0x0000;
    *bpb.FSVer() = 0x0000;
    *bpb.rootDirClustId() = 2;
    *bpb.FSISecId() = 1;
    *bpb.DBRBakSecId() = 6;
    *bpb.BIOSDrive() = 0x80;
    *bpb.EDPBSig() = 0x29;
    *bpb.serial() = 0x12345678; // should be random
    memcpy(bpb.label(), "           ", 11);
    memcpy(bpb.FSLabel(), "FAT32   ", 8);
    // backup DBR
    memcpy(m_data + *bpb.bytesPerSec() * *bpb.DBRBakSecId(), m_data, 512);
    // set FAT
    for (uint8_t i = 0; i < *bpb.FATNum(); i++)
        FAT(i).init();

    return true;
}