#include <stdio.h>
#include "mydisk.h"
#include "myfat32.h"
#include "myutil.h"

char file[] = "E:\\test\\new.img";

int main()
{
    FILE *f;
    MyDisk disk;
    MyPartition part;
    // try clear
    f = fopen(file, "w+");
    if(f)
        fclose(f);
    disk.load(file, 0, 512 * 4096);
    disk.init();
    disk.newPartition(0, 63, 128457);
    part = disk.partition(0);
    MyFAT32 fat32(part, 0, part.len());
    fat32.format();
    fat32.info();
    disk.save(file);
    return 0;
}