#ifndef _BPB_H_
#define _BPB_H_

#include "datahandle.h"

class BPB_t : public DataHandle
{
public:
    // general
    using DataHandle::DataHandle;

    //data
    uint16_t *bytesPerSec() { return (uint16_t *)(m_data + 0); }    // len: 2 // {512, 1024, 2048, 4096}
    uint8_t *secPerClust() { return (m_data + 2); }                 // len: 1 // {1, 2, 4, 8, 16, 32, 64, 128} // max(bytesPerClust) is always 32768, 65536 in rare cases
    uint16_t *reservedSecNum() { return (uint16_t *)(m_data + 3); } // len: 2 // 32 for FAT32
    uint8_t *FATNum() { return (m_data + 5); }                      // len: 1 // always 2
    // uint16_t rootDirNum;                                         // len: 2 // 0 for FAT32
    // uint16_t secNum_small;                                       // len: 2 // unused in FAT32
    uint8_t *mediaType() { return (m_data + 10); } // len: 1 // 0xF8: hard disk 0xFA: RAM disk
    // uint16_t secPerFAT;                         // len: 2 // 0 for FAT32
    uint16_t *secPerTrack() { return (uint16_t *)(m_data + 13); }    // len: 2
    uint16_t *headNum() { return (uint16_t *)(m_data + 15); }        // len: 2
    uint32_t *hiddenSecNum() { return (uint32_t *)(m_data + 17); }   // len: 4 // equals to DPT_item->startSec
    uint32_t *secNum() { return (uint32_t *)(m_data + 21); }         // len: 4
    uint32_t *FATSize() { return (uint32_t *)(m_data + 25); }        // len: 4 // in sectors
    uint16_t *FATflag() { return (uint16_t *)(m_data + 29); }        // len: 2 // FATflag[7]=0 to update all FATs
    uint16_t *FSVer() { return (uint16_t *)(m_data + 31); }          // len: 2 // 0
    uint32_t *rootDirClustId() { return (uint32_t *)(m_data + 33); } // len: 4 // usually 2
    uint16_t *FSISecId() { return (uint16_t *)(m_data + 37); }       // len: 2 // usually 1
    uint16_t *DBRBakSecId() { return (uint16_t *)(m_data + 39); }    // len: 2 // usually 6
    // uint8_t reserved[12];                                         // len: 12
    uint8_t *BIOSDrive() { return (m_data + 53); } // len: 1 // logical drive number(for INT 13), 0x80
    // uint8_t reserved;                           // len: 1
    uint8_t *EDPBSig() { return (m_data + 55); }             // len: 1 // 0x29
    uint32_t *serial() { return (uint32_t *)(m_data + 56); } // len: 4
    uint8_t *label() { return (m_data + 60); }               // len: 11 // fill space(0x20)
    uint8_t *FSLabel() { return (m_data + 71); }             // len: 8 // "FAT32   "

    // extended data
    uint64_t mappedDataSectors();
    uint64_t physicalDataSectors();
    uint64_t validDataSectors();
    uint64_t validFATItems();

    // operations
    bool isValid() override;
    void info() override;
};

#endif