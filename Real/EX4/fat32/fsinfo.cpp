#include "fsinfo.h"

bool FSInfo_t::isValid()
{
    return DataHandle::isValid() && *(uint32_t *)end() == 0xAA550000;
}

void FSInfo_t::info()
{
    uint8_t *jmpData;
    uint8_t *OEMData;

    DataHandle::info();
    printf("FSInfo_t\n");
    printf("Signature1: 0x%08x < - > 0x41615252\n", *sig1());
    printf("Signature2: 0x%08x < - > 0x61417272\n", *sig2());
    printf("Free Cluster Num: %u\n", *freeClustNum());
    printf("Next Empty Cluster: %u\n", *nextEmptyClust());
}

bool FSInfo_t::init(BPB_t &bpb, FAT_t &fat)
{
    if (!DataHandle::isValid())
        return false;

    fill(0x00);
    *sig1() = 0x41615252;
    *sig2() = 0x61417272;
    *freeClustNum() = bpb.validDataSectors() / *bpb.secPerClust();
    *nextEmptyClust() = fat.nextEmpty();
    *end() = 0xAA550000;
    return true;
}