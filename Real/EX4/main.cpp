#include <stdio.h>
#include <ctype.h>
#include "mydisk.h"
#include "myutil.h"
#include "myfat32.h"
#include "mydir.h"
#include "myfile.h"

char buf1[256], buf2[256];
char path[256];
char cmd[256];
uint8_t buf[4096];
uint64_t diskLen;

int main()
{
    MyDisk disk;
    MyPartition part;

    printf("Disk File:\n");
    scanf("%s", path);
    FILE *f = fopen(path, "rb");
    if (f == nullptr)
    {
        printf("Failed to open disk file!\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);
    diskLen = ftell(f);
    fclose(f);
    printf("Disk size(bytes):%llu\n", diskLen);
    if (!disk.load(path, 0, diskLen))
    {
        printf("Failed to load disk data!\n");
        return 0;
    }

    part = disk.partition(0);
    MyFAT32 fat32(part, 0, 0);
    MyDir_t dir(fat32);
    MyFile_t file(fat32);
    while (1)
    {
        dir.pwd(buf1);
        printf("%s>", buf1);
        scanf("%s", cmd);
        for (uint8_t i = 0; cmd[i] != '\0'; i++)
            cmd[i] = tolower(cmd[i]);

        if (strcmp(cmd, "ls") == 0)
        {
            printf("ls:%u\n", dir.ls());
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            scanf("%s", buf1);
            printf("target:%s\n", buf1);
            printf("cd:%u\n", dir.cd(buf1));
        }
        else if (strcmp(cmd, "pwd") == 0)
        {
            dir.pwd(buf1);
            printf("%s\n", buf1);
        }
        else if (strcmp(cmd, "mkdir") == 0)
        {
            scanf("%s", buf1);
            printf("target:%s\n", buf1);
            printf("mkdir:%u\n", dir.mkdir(buf1));
        }
        else if (strcmp(cmd, "mkfile") == 0)
        {
            uint32_t len;
            scanf("%s%u", buf1, &len);
            printf("target:%s\n", buf1);
            printf("mkfile:%u\n", dir.mkfile(buf1, len));
        }
        else if (strcmp(cmd, "rename") == 0)
        {
            scanf("%s", buf1);
            scanf("%s", buf2);
            printf("rename:%u\n", dir.rename(buf1, buf2));
        }
        else if (strcmp(cmd, "save") == 0)
        {
            fat32.syncFAT();
            printf("save:%u\n", disk.save(path));
        }
        else if (strcmp(cmd, "open") == 0)
        {
            scanf("%s", buf1);
            if (buf1[0] == '/') //absolute buf1;
            {
                printf("open:%u\n", file.open(buf1));
            }
            else // relative buf1
            {
                dir.pwd(buf2);
                strcat(buf2, buf1);
                printf("open:%u\n", file.open(buf2));
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
        else if (strcmp(cmd, "write") == 0)
        {
            uint64_t offset, len;
            scanf("%llu%s", &offset, buf);
            len = file.write(buf, offset, strlen((char *)buf));
            printf("write:%llu\n", len);
        }
        else if (strcmp(cmd, "resize") == 0)
        {
            uint64_t len;
            scanf("%llu", &len);
            printf("resize:%u\n", file.resize(len));
        }
        else if (strcmp(cmd, "remove") == 0 || strcmp(cmd, "rm") == 0)
        {
            scanf("%s", buf1);
            printf("remove:%u\n", dir.remove(buf1));
        }
        else if (strcmp(cmd, "info") == 0)
        {
            fat32.info();
        }
        else if (strcmp(cmd, "format") == 0)
        {
            uint8_t spc;
            scanf("%u", &spc);
            printf("format:%u\n", fat32.format(spc));
        }
        else if (strcmp(cmd, "shrink") == 0)
        {
            printf("shrink:%u\n", dir.tryShrink());
        }
        else if (strcmp(cmd, "showfat") == 0)
        {
            uint64_t len;
            scanf("%u", &len);
            printf("FAT hex data:\n");
            fat32.FAT(0).showFAT(len);
        }
        else if (strcmp(cmd, "q") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0)
        {
            break;
        }
    }
    return 0;
}