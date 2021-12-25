#include "myfat32.h"
#include "string.h"
#include <stdio.h>

bool MyFAT32::isValid()
{
    return MyPartition::isValid() && DBR().isValid();
}

void MyFAT32::info()
{
    DataHandle::info();
    printf("MyFAT32\n");
    DBR().info();
}

bool DBR_t::isValid()
{
    return DataHandle::isValid() && *(uint16_t *)end() == 0xAA55;
}

void DBR_t::info()
{
    uint8_t *jmpData;
    uint8_t *OEMData;
    BPB_t bpb;

    DataHandle::info();
    printf("DBR\n");
    jmpData = jmp();
    OEMData = OEM();
    bpb = BPB();
    printf("jmp:0x%02x 0x%02x 0x%02x <-> 0xEB 0x58 0x90\n", jmpData[0], jmpData[1], jmpData[2]);
    printf("OEM:");
    for (uint8_t i = 0; i < 8; i++)
        putchar(OEMData[i]);
    putchar('\n');
    bpb.info();
    // Check backup?
}

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
    if (!isValid())
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
    printf("EDPBSig:%u\n", *EDPBSig());
    printf("serial:%u\n", *serial());
    printf("label:");
    for (uint8_t i = 0; i < 11; i++)
        putchar(labelData[i]);
    putchar('\n');
    printf("FSLabel:");
    for (uint8_t i = 0; i < 8; i++)
        putchar(FSLabelData[i]);
    putchar('\n');
}