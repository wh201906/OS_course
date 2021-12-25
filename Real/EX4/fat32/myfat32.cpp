#include "myfat32.h"
#include "string.h"

void FAT32_getBPB(const FAT32_DBR_t *DBR, FAT32_BPB_t *BPB)
{
    uint8_t *ptr = (uint8_t *)(DBR->BPBData);
    BPB->bytesPerSec = *(uint16_t *)(ptr + 0);
    BPB->bytesPerSec = *(uint16_t *)(ptr + 0);
    BPB->secPerClust = *(ptr + 2);
    BPB->reservedSecNum = *(uint16_t *)(ptr + 3);
    BPB->FATNum = *(ptr + 5);
    BPB->mediaType = *(ptr + 10);
    BPB->secPerTrack = *(uint16_t *)(ptr + 13);
    BPB->headNum = *(uint16_t *)(ptr + 15);
    BPB->hiddenSecNum = *(uint32_t *)(ptr + 17);
    BPB->secNum = *(uint32_t *)(ptr + 21);
    BPB->FATSize = *(uint32_t *)(ptr + 25);
    BPB->FATflag = *(uint16_t *)(ptr + 29);
    BPB->FSVer = *(uint16_t *)(ptr + 31);
    BPB->rootDirClustId = *(uint32_t *)(ptr + 33);
    BPB->FSISecId = *(uint16_t *)(ptr + 37);
    BPB->DBRBakSecId = *(uint16_t *)(ptr + 39);
    BPB->BIOSDrive = *(ptr + 53);
    BPB->EDPBSig = *(ptr + 55);
    BPB->serial = *(uint32_t *)(ptr + 56);
    memcpy(BPB->label, ptr + 60, 11);
    memcpy(BPB->FSLabel, ptr + 71, 8);
}

void FAT32_setBPB(FAT32_DBR_t *DBR, const FAT32_BPB_t *BPB)
{
    uint8_t *ptr = (uint8_t *)(DBR->BPBData);
    *(uint16_t *)(ptr + 0) = BPB->bytesPerSec;
    *(uint16_t *)(ptr + 0) = BPB->bytesPerSec;
    *(ptr + 2) = BPB->secPerClust;
    *(uint16_t *)(ptr + 3) = BPB->reservedSecNum;
    *(ptr + 5) = BPB->FATNum;
    *(ptr + 10) = BPB->mediaType;
    *(uint16_t *)(ptr + 13) = BPB->secPerTrack;
    *(uint16_t *)(ptr + 15) = BPB->headNum;
    *(uint32_t *)(ptr + 17) = BPB->hiddenSecNum;
    *(uint32_t *)(ptr + 21) = BPB->secNum;
    *(uint32_t *)(ptr + 25) = BPB->FATSize;
    *(uint16_t *)(ptr + 29) = BPB->FATflag;
    *(uint16_t *)(ptr + 31) = BPB->FSVer;
    *(uint32_t *)(ptr + 33) = BPB->rootDirClustId;
    *(uint16_t *)(ptr + 37) = BPB->FSISecId;
    *(uint16_t *)(ptr + 39) = BPB->DBRBakSecId;
    *(ptr + 53) = BPB->BIOSDrive;
    *(ptr + 55) = BPB->EDPBSig;
    *(uint32_t *)(ptr + 56) = BPB->serial;
    memcpy(ptr + 60, BPB->label, 11);
    memcpy(ptr + 71,BPB->FSLabel, 8);
}