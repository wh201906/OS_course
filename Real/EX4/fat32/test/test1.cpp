#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"
#include "myfat32.h"

#define LEN (512 * 1024)

uint8_t globalBuf[LEN];

int main()
{
    MBR_t *mbr;
    DPT_item dpt;
    FAT32_DBR_t *dbr;
    FAT32_BPB_t bpb;
    uint32_t start, len;

    // read disk, find first partition
    file2buf("E:\\test\\disk.img", globalBuf, 0, LEN);
    mbr = (MBR_t *)globalBuf;
    Disk_getDPT(mbr, &dpt, 0);
    start = dpt.startSec;
    len = dpt.secSize;

    dbr = (FAT32_DBR_t *)(globalBuf + start * 512);
    FAT32_getBPB(dbr, &bpb);

    return 0;
}