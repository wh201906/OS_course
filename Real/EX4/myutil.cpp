#include "myutil.h"

void file2buf(const char* filename, uint8_t* buf, uint64_t fileOffset, uint64_t len)
{
    FILE *file = fopen(filename, "rb");
    fseek(file, fileOffset, SEEK_SET);
    fread(buf, 1, len, file);
}

void buf2file(const char* filename, uint8_t* buf, uint64_t fileOffset, uint64_t len)
{
    FILE *file = fopen(filename, "wb");
    fseek(file, fileOffset, SEEK_SET);
    fwrite(buf, 1, len, file);
}

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

