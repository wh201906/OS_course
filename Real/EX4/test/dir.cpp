#include <stdio.h>
#include "mydisk.h"
#include "myutil.h"
#include "myfat32.h"
#include "mydir.h"

int main()
{
    char targetPath[50];
    char cmd[50];
    MyDisk disk;
    MyPartition part;
    disk.load("E:\\test\\disk1.img", 0, 512 * 8192 * 16);
    part = disk.partition(0);
    MyFAT32 fat32(part, 0, 0);
    MyDir_t dir(fat32);
    while (1)
    {
        char currPath[128];
        dir.pwd(currPath);
        printf("%s>", currPath);
        scanf("%s", cmd);
        if (strcmp(cmd, "ls") == 0)
        {
            printf("ls:%u\n", dir.ls());
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            scanf("%s", targetPath);
            printf("target:%s\n", targetPath);
            printf("cd:%u\n", dir.cd(targetPath));
        }
        else if (strcmp(cmd, "pwd") == 0)
        {
            dir.pwd(currPath);
            printf("%s\n", currPath);
        }
        else if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0)
        {
            break;
        }
    }
    return 0;
}