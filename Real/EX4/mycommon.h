#ifndef _MYCOMMON_H_
#define _MYCOMMON_H_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

#define HPC 255
#define SPT 63

struct CHS_t
{
    uint64_t C;
    uint8_t H;
    uint8_t S;
};

#endif