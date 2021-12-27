#include "bpb.h"

bool BPB_t::isValid()
{
    bool result = true;
    result &= DataHandle::isValid();
    result &= *bytesPerSec() > 0;
    result &= *secPerClust() > 0;
    result &= *secNum() > 0;
    return result;
}

void BPB_t::info()
{
    uint8_t *labelData;
    uint8_t *FSLabelData;

    DataHandle::info();
    printf("BPB_t\n");
    if (!DataHandle::isValid())
        return;

    labelData = label();
    FSLabelData = FSLabel();
    printf("bytesPerSec:%u\n", *bytesPerSec());
    printf("secPerClust:%u\n", *secPerClust());
    printf("reservedSecNum:%u\n", *reservedSecNum());
    printf("FATNum:%u\n", *FATNum());
    printf("mediaType:0x%02x\n", *mediaType());
    printf("secPerTrack:%u\n", *secPerTrack());
    printf("headNum:%u\n", *headNum());
    printf("hiddenSecNum:%u\n", *hiddenSecNum());
    printf("secNum:%u\n", *secNum());
    printf("FATSize:%u\n", *FATSize());
    printf("FATflag:0x%04x\n", *FATflag());
    printf("FSVer:%u\n", *FSVer());
    printf("rootDirClustId:%u\n", *rootDirClustId());
    printf("FSISecId:%u\n", *FSISecId());
    printf("DBRBakSecId:%u\n", *DBRBakSecId());
    printf("BIOSDrive:%u\n", *BIOSDrive());
    printf("EDPBSig:0x%02x\n", *EDPBSig());
    printf("serial:%u\n", *serial());
    printf("label:");
    for (uint8_t i = 0; i < 11; i++)
        putchar(labelData[i]);
    putchar('\n');
    printf("FSLabel:");
    for (uint8_t i = 0; i < 8; i++)
        putchar(FSLabelData[i]);
    putchar('\n');
    putchar('\n');
    uint64_t pos, size;
    uint64_t offset = *hiddenSecNum();

    printf("Map:(diskPos, partitionPos, size) in sectors\n");
    printf("|");
    pos = 0;
    size = *reservedSecNum();
    printf("  Reserved(%u,%u,%u)  |", pos + offset, pos, size);
    for (uint8_t i = 0; i < *FATNum(); i++)
    {
        pos += size;
        size = *FATSize();
        printf("  FAT%u(%u,%u,%u)  |", i, pos + offset, pos, size);
    }
    pos += size;
    uint64_t phy, mapped;
    phy = physicalDataSectors();
    mapped = mappedDataSectors();
    if (phy > mapped)
    {
        printf("  Data(%u,%u,%u)  |", pos + offset, pos, mapped);
        pos += mapped;
        printf("  Unused*(%u,%u,%u)  |", pos + offset, pos, phy - mapped);
        putchar('\n');
        printf("*: This area exists, but can't be mapped from FAT.\n");
    }
    else if (phy < mapped)
    {
        printf("  Data(%u,%u,%u)  |", pos + offset, pos, phy);
        pos += phy;
        printf("  Invalid*(%u,%u,%u)  |", pos + offset, pos, mapped - phy);
        putchar('\n');
        printf("*: This area can be mapped from FAT, but doesn't exist.\n");
    }
}

// two siturations:
// let x = (FATSize * BPS / 4 - 2) * SPC
// x stands for the sectors that FAT can mapped
// let y = secNum - reservedSecNum - FATSize * FATNum
// y stands for actural sectors for data
// if x = y, everything is fine
// if x > y, some FAT items are invalid
// if x < y, some sectors won't be used.
// my MyFAT32::format() will make sure x <= y
uint64_t BPB_t::mappedDataSectors()
{
    return ((*FATSize() * *bytesPerSec() / 4 - 2) * *secPerClust());
}
uint64_t BPB_t::physicalDataSectors()
{
    return (*secNum() - *reservedSecNum() - *FATNum() * *FATSize());
}
uint64_t BPB_t::validDataSectors()
{
    uint64_t phy, mapped;
    phy = physicalDataSectors();
    mapped = mappedDataSectors();
    return (phy < mapped ? phy : mapped);
}
uint64_t BPB_t::validFATItems()
{
    uint64_t fromFAT, fromPhy;
    fromFAT = *FATSize() - 2;
    fromPhy = physicalDataSectors() * 4.0 / *secPerClust() / *bytesPerSec(); // floor()
    return (fromFAT < fromPhy ? fromFAT : fromPhy);
}