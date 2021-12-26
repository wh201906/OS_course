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
    uint64_t pos, size, unUsedSize;
    uint64_t offset = *hiddenSecNum();
    unUsedSize = *secNum() + 2 * *secPerClust();
    unUsedSize -= *FATNum() * *FATSize();
    unUsedSize -= *bytesPerSec() * *secPerClust() * *FATSize() / 4;

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
    size = *secNum() - pos - unUsedSize;
    printf("  Data(%u,%u,%u)  |", pos + offset, pos, size);
    pos += size;
    printf("  Unused*(%u,%u,%u)  |", pos + offset, pos, unUsedSize);
    putchar('\n');
    printf("*: This area exists, but can't be mapped from FAT.\n");
}