#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"

uint8_t globalBuf[512];

int main()
{
    MBR_t *mbr;
    DPT_item dpt[4];

    CHS_t newStartCHS, newEndCHS;
    uint32_t newStartSec, newEndSec;
    
    // analyze
    file2buf("E:\\test\\disk.img", globalBuf, 0, 512);
    mbr = (MBR_t *)globalBuf;
    for (int i = 0; i < 4; i++)
        Disk_getDPT(mbr, &dpt[i], i);

    // convert
    newStartCHS = LBA2CHS(dpt[0].startSec);
    newEndCHS = LBA2CHS(dpt[0].startSec + dpt[0].secSize - 1);
    newStartSec = CHS2LBA(dpt[0].startCHS);
    newEndSec = CHS2LBA(dpt[0].endCHS);

    printf("Origin:\n");
    printf("    LBA:\n");
    printf("    %u + %u - 1 = %u\n", dpt[0].startSec, dpt[0].secSize, dpt[0].startSec + dpt[0].secSize - 1);
    printf("    CHS:\n");
    printf("    %llu, %u, %u\n", dpt[0].startCHS.C, dpt[0].startCHS.H, dpt[0].startCHS.S);
    printf("    %llu, %u, %u\n", dpt[0].endCHS.C, dpt[0].endCHS.H, dpt[0].endCHS.S);

    printf("Converted:\n");
    printf("    LBA:\n");
    printf("    %u, %u\n", newStartSec, newEndSec);
    printf("    CHS:\n");
    printf("    %llu, %u, %u\n", newStartCHS.C, newStartCHS.H, newStartCHS.S);
    printf("    %llu, %u, %u\n", newEndCHS.C, newEndCHS.H, newEndCHS.S);

    return 0;
}