#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"

int main()
{
    MyDisk disk;
    disk.load("E:\\test\\disk.img", 0, 512 * 128);
    disk.info();
    disk.showHex(1020);
    return 0;
}