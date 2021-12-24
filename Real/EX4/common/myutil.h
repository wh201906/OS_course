#ifndef _MYUTIL_H_
#define _MYUTIL_H_

#include "mycommon.h"

void file2buf(const char *filename, uint8_t *buf, uint64_t fileOffset, uint64_t len);
void buf2file(const char *filename, uint8_t *buf, uint64_t fileOffset, uint64_t len);

CHS_t LBA2CHS(uint64_t LBA);
uint64_t CHS2LBA(CHS_t CHS);

#endif