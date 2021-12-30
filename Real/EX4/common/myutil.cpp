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

std::vector<std::string> split(const char *str, const char *splitter)
{
    // use regex unmatched(-1 for 4th arg) result for splitting
    std::string pathStr(str);
    std::regex re(splitter);
    return std::vector<std::string>(std::sregex_token_iterator(pathStr.begin(), pathStr.end(), re, -1), std::sregex_token_iterator());
}

void printHex(uint8_t *data, uint64_t len)
{
    for (uint64_t i = 0; i < len; i++)
    {
        printf("%02x ", *(data + i));
        if (i % 16 == 15)
            printf("\n");
        else if (i % 8 == 7)
            printf("  ");
    }
    printf("\n");
}