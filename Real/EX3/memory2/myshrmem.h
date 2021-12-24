#ifndef MYSHRMEM_H
#define MYSHRMEM_H

#include <qglobal.h>

typedef struct _mymsg
{
    uint64_t len1;
    uint8_t data1[128];
    uint64_t len2;
    uint8_t data2[128];
} mymsg;

#endif // MYSHRMEM_H
