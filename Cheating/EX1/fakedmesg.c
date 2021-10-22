#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFLEN 128
typedef struct timespec timStrc;

void printk_fake(double *timestamp, char *src, char *msg)
{
    double time;
    if (timestamp != NULL)
    {
        time = *timestamp;
    }
    else
    {
        timStrc ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        time = ts.tv_nsec / 1000000000.0;
        time += ts.tv_sec;
    }
    printf("\033[22;32m"); // green, normal
    printf("[%12.6f]", time);
    printf("\033[0m"); // cancel

    if (src != NULL) // no bold
    {
        printf(" ");
        printf("\033[22;33m"); // yellow, normal
        printf("%s", src);

        printf("\033[22;37m"); // white, normal
        printf(": %s", msg);
        printf("\033[0m"); // cancel
    }
    else
    {
        printf("\033[1;37m"); // white, bold
        printf(" %s", msg);
        printf("\033[0m"); // cancel
    }

    printf("\n");
}

int main()
{
    char lineBuf[BUFLEN];
    char *ptr, *srcPtr;
    int i, restLen;
    double time;
    system("/usr/bin/dmesg");
    freopen("msg.txt", "rt", stdin);
    while (fgets(lineBuf, BUFLEN, stdin) != NULL)
    {
        for (i = 0; i < BUFLEN; i++) // i is always less than BUFLEN, because '\0' must exist.
        {
            if (lineBuf[i] == '\n' || lineBuf[i] == '\0')
                break;
        }

        if (i == 0)
            continue;
        lineBuf[i] = '\0';
        ptr = lineBuf + 2;
        restLen = i - 2;

        if (lineBuf[0] == '1')
        {
            printk_fake(NULL, NULL, ptr);
        }
        else if (lineBuf[0] == '2')
        {
            for (i = 0; i < restLen; i++)
            {
                if (ptr[i] == '^') // separator
                    break;
            }
            if (i == 0 || i == restLen)
                continue;
            ptr[i] = '\0';
            time = atof(ptr);
            ptr += i + 1;
            printk_fake(&time, NULL, ptr);
        }
        else if (lineBuf[0] == '3')
        {
            srcPtr = ptr;
            for (i = 0; i < restLen; i++)
            {
                if (ptr[i] == '^') // separator
                    break;
            }
            if (i == 0 || i == restLen)
                continue;
            ptr[i] = '\0';
            ptr += i + 1;
            printk_fake(NULL, srcPtr, ptr);
        }
        else if (lineBuf[0] == '4')
        {
            for (i = 0; i < restLen; i++)
            {
                if (ptr[i] == '^') // separator
                    break;
            }
            if (i == 0 || i == restLen)
                continue;
            ptr[i] = '\0';
            time = atof(ptr);
            ptr += i + 1;
            restLen -= i + 1;
            srcPtr = ptr;
            for (i = 0; i < restLen; i++)
            {
                if (ptr[i] == '^') // separator
                    break;
            }
            if (i == 0 || i == restLen)
                continue;
            ptr[i] = '\0';
            ptr += i + 1;
            printk_fake(&time, srcPtr, ptr);
        }
    }
    return 0;
}
