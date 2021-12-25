#ifndef _MYFAT32_H_
#define _MYFAT32_H_

#include "mycommon.h"

// 25 fields
// 24 valid fields in FAT32
struct FAT32_BPB_t
{
    uint16_t bytesPerSec;
    uint8_t secPerClust;
    uint16_t reservedSecNum;
    uint8_t FATNum;
    // uint16_t rootDirNum;
    // uint16_t secNum_small;
    uint8_t mediaType;
    // uint16_t secPerFAT;
    uint16_t secPerTrack;
    uint16_t headNum;
    uint32_t hiddenSecNum; // equals to DPT_item->startSec
    uint32_t secNum;
    uint32_t FATSize; // in sectors
    uint16_t FATflag;
    uint16_t FSVer;
    uint32_t rootDirClustId;
    uint16_t FSISecId;
    uint16_t DBRBakSecId;
    // uint8_t reserved[12];
    uint8_t BIOSDrive;
    // uint8_t reserved;
    uint8_t EDPBSig;
    uint32_t serial;
    uint8_t label[11];
    uint8_t FSLabel[8];
};

// mapped to the raw disk data
// sizeof(FAT32_DBR_t) = 512;
struct FAT32_DBR_t
{
    uint8_t jmp[3] = {0xEB, 0x58, 0x90};
    uint8_t OEM[8] = {'M', 'S', 'D', 'O', 'S', '5', '.', '0'};
    uint8_t BPBData[79];
    uint8_t bootCode[420];
    uint8_t end[2] = {0x55, 0xAA};
};

void FAT32_getBPB(const FAT32_DBR_t *DBR, FAT32_BPB_t *BPB);
void FAT32_setBPB(FAT32_DBR_t *DBR, const FAT32_BPB_t *BPB);

#endif