#include "myutil.h"
#include <stdio.h>

CHS_t LBA2CHS(uint64_t LBA)
{
    CHS_t result;
    result.C = LBA / (HPC * SPT);
    result.H = (LBA / SPT) % HPC;
    result.S = (LBA % SPT) + 1;
    return result;
}

uint64_t CHS2LBA(CHS_t CHS)
{
    return (CHS.C * HPC + CHS.H) * SPT + CHS.S - 1;
}

