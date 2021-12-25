#include "datahandle.h"

int main()
{
    DataHandle ori, mbr, dbr;

    ori.load("E:\\test\\disk.img", 0, 512 * 128);
    mbr.mapTo(ori, 0, 512);
    dbr.mapTo(ori, 63 * 512, 512);
    ori.save("E:\\test\\new.img");
    mbr.save("E:\\test\\mbr.img");
    dbr.save("E:\\test\\dbr.img");

    return 0;
}