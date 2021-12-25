#ifndef _MYUTIL_H_
#define _MYUTIL_H_

#include "mycommon.h"

CHS_t LBA2CHS(uint64_t LBA);
uint64_t CHS2LBA(CHS_t CHS);

#endif