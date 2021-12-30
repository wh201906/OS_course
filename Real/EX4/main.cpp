#include <stdio.h>
#include <ctype.h>
#include "mydisk.h"
#include "myutil.h"
#include "myfat32.h"
#include "mydir.h"
#include "myfile.h"

char path[256];
char cmd[256];
uint8_t buf[4096];

int main()
{
    MyDisk disk;
    MyPartition part;
    disk.load("E:\\test\\disk1.img", 0, 512 * 8192 * 16);
    part = disk.partition(0);
    MyFAT32 fat32(part, 0, 0);
    MyDir_t dir(fat32);
    MyFile_t file(fat32);
    while (1)
    {
        dir.pwd(path);
        printf("%s>", path);
        scanf("%s", cmd);
        for (uint8_t i = 0; cmd[i] != '\0'; i++)
            cmd[i] = tolower(cmd[i]);

        if (strcmp(cmd, "ls") == 0)
        {
            printf("ls:%u\n", dir.ls());
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            scanf("%s", path);
            printf("target:%s\n", path);
            printf("cd:%u\n", dir.cd(path));
        }
        else if (strcmp(cmd, "pwd") == 0)
        {
            dir.pwd(path);
            printf("%s\n", path);
        }
        else if (strcmp(cmd, "mkdir") == 0)
        {
            scanf("%s", path);
            printf("target:%s\n", path);
            printf("mkdir:%u\n", dir.mkdir(path));
        }
        else if (strcmp(cmd, "rename") == 0)
        {
            scanf("%s", path);
            scanf("%s", cmd);
            printf("rename:%u\n", dir.rename(path, cmd));
        }
        else if (strcmp(cmd, "save") == 0)
        {
            printf("save:%u\n", disk.save("E:\\test\\disk1.img"));
        }
        else if (strcmp(cmd, "open") == 0)
        {
            scanf("%s", path);
            if (path[0] == '/') //absolute path;
            {
                printf("open:%u\n", file.open(path));
            }
            else // relative path
            {
                dir.pwd(cmd);
                strcat(cmd, path);
                printf("open:%u\n", file.open(cmd));
            }
        }
        else if (strcmp(cmd, "read") == 0)
        {
            uint64_t len, offset;
            scanf("%llu%llu", &offset, &len);
            len = file.read(buf, offset, len);
            printf("read:%llu\n", len);
            printHex(buf, len);
        }
        else if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0)
        {
            break;
        }
    }
    return 0;
}