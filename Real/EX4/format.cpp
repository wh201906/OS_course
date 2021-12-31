#include <stdio.h>
#include "mydisk.h"
#include "myfat32.h"
#include "myutil.h"

const uint64_t offset = 63;
const uint8_t dptID = 0;
char path[256];
uint64_t diskLen;
uint32_t spc;

int main()
{
    bool opened = false;
    MyDisk disk;
    MyPartition part;

    printf("Disk File:\n");
    scanf("%s", path);
    printf("Disk size(bytes):\n");
    scanf("%llu", &diskLen);
    opened = disk.load(path, 0, diskLen);
    if (!opened)
    {
        printf("File doesn't exist. Creating...\n");
        fclose(fopen(path, "w+"));
        if (!disk.load(path, 0, diskLen))
        {
            printf("Failed to create file!\n");
            return 0;
        }
        else
            printf("File created.\n");
    }

    printf("Creating disk...\n");
    disk.init();
    printf("Creating partition at %llu sector, length: %llu sectors, using DPT %u...\n", offset, diskLen / BPS - offset - 1, dptID);
    disk.newPartition(dptID, offset, diskLen / BPS - offset - 1);
    part = disk.partition(dptID);
    MyFAT32 fat32(part, 0, part.len());
    printf("Sectors per cluster?\n");
    scanf("%u", &spc);
    printf("Formatting...\n");
    if (!fat32.format(spc))
    {
        printf("Failed to format!\n");
        return 0;
    }
    printf("Formatted.\n");
    printf("Partition info:\n");
    fat32.info();
    printf("Saving to file...\n");
    disk.save(path);
    printf("Saved.");
    return 0;
}