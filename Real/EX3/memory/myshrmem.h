#ifndef MYSHRMEM_H
#define MYSHRMEM_H

#include <qglobal.h>

typedef struct _mymsg
{
    uint64_t len;
    uint8_t data[];
} mymsg;

#endif // MYSHRMEM_H
