#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"
#include "myfat32.h"

#define LEN (512 * 1024)

uint8_t globalBuf[LEN];

int main()
{
    MyDisk disk;
    MyPartition part;
    disk.load("E:\\test\\disk.img", 0, 512 * 128);
    part = disk.partition(0);
    MyFAT32 fat32(part, 0, 0);
    fat32.info();

    return 0;
}