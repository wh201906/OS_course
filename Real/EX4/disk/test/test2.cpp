#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"

uint8_t globalBuf[512];

int main()
{
    MBR_t *mbr;
    DPT_item dpt[4], newdpt[4];
    
    // analyze
    file2buf("E:\\test\\disk.img", globalBuf, 0, 512);
    mbr = (MBR_t *)globalBuf;
    for (int i = 0; i < 4; i++)
        Disk_getDPT(mbr, &dpt[i], i);

    printf("Origin:\n");
    printf("    LBA:\n");
    printf("    %u + %u - 1 = %u\n", dpt[0].startSec, dpt[0].secSize, dpt[0].startSec + dpt[0].secSize - 1);
    printf("    CHS:\n");
    printf("    %llu, %u, %u\n", dpt[0].startCHS.C, dpt[0].startCHS.H, dpt[0].startCHS.S);
    printf("    %llu, %u, %u\n", dpt[0].endCHS.C, dpt[0].endCHS.H, dpt[0].endCHS.S);

    // copy
    Disk_setDPT(mbr, &dpt[0], 1);
    for (int i = 0; i < 4; i++)
        Disk_getDPT(mbr, &newdpt[i], i);

    printf("New:\n");
    printf("    LBA:\n");
    printf("    %u + %u - 1 = %u\n", newdpt[1].startSec, newdpt[1].secSize, newdpt[1].startSec + newdpt[1].secSize - 1);
    printf("    CHS:\n");
    printf("    %llu, %u, %u\n", newdpt[1].startCHS.C, newdpt[1].startCHS.H, newdpt[1].startCHS.S);
    printf("    %llu, %u, %u\n", newdpt[1].endCHS.C, newdpt[1].endCHS.H, newdpt[1].endCHS.S);


    return 0;
}