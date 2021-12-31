#ifndef _MYCOMMON_H_
#define _MYCOMMON_H_

#include <stdio.h>
#include <stdint.h>

// heads per cylinder
#define HPC 255
// sectors per head/track
#define SPT 63
// bytes per sector
#define BPS 512

#define PATHLEN 256

struct CHS_t
{
    uint64_t C;
    uint8_t H;
    uint8_t S;
};

const char justSpaces[] = "            ";

#endif