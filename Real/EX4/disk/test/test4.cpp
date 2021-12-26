#include <stdio.h>
#include "mydisk.h"
#include "myfat32.h"
#include "myutil.h"

int main()
{
    MyDisk disk;
    MyPartition part;
    disk.load("E:\\test\\new.img", 0, 512 * 4096);
    disk.init();
    disk.newPartition(0, 63, 128457);
    part = disk.partition(0);
    MyFAT32 fat32(part, 0, part.len());
    fat32.format();
    fat32.info();
    disk.save("E:\\test\\new.img");
    return 0;
}