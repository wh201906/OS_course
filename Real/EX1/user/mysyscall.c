#include "mysyscall.h"
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <string.h>

void Syscall_Test()
{
    char data = '\x00';
    syscall(MYSYSCALL_ID, &data, 1);
}

void Syscall_Show(char *str)
{
    int len = 0, i;
    char data[512];
    data[0] = '\x01';

    while (str[len] != '\0')
        len++;
    for (i = 0; i < len; i++)
        data[1 + i] = str[i];

    data[1 + len++] = '\0';
    syscall(MYSYSCALL_ID, data, len + 1);
}

void Syscall_GetNice(int pid, int *nice, int *prio)
{
    char data[16];
    data[0] = '\x02';
    *(int *)(data + 1) = pid;
    syscall(MYSYSCALL_ID, data, 9);
    if (nice != NULL)
        *nice = *(int *)(data + 0);
    if (prio != NULL)
        *prio = *(int *)(data + 8);
}

void Syscall_SetNice(int pid, int nice, int *oldNice, int *oldPrio, int *newNice, int *newPrio)
{
    char data[32];
    data[0] = '\x03';
    *(int *)(data + 1) = pid;
    *(int *)(data + 9) = nice;
    syscall(MYSYSCALL_ID, data, 17);
    if (oldNice != NULL)
        *oldNice = *(int *)(data + 0);
    if (oldPrio != NULL)
        *oldPrio = *(int *)(data + 8);
    if (newNice != NULL)
        *newNice = *(int *)(data + 16);
    if (newPrio != NULL)
        *newPrio = *(int *)(data + 24);
}

void Syscall_GetHostname(char *result)
{
    int i = 0;
    char data[512];
    data[0] = '\x04';
    syscall(MYSYSCALL_ID, data, 1);
    if (result != NULL)
    {
        while (data[i] != '\0')
        {
            result[i] = data[i];
            i++;
        }
        result[i] = '\0';
    }
}

void Syscall_SetHostname(char *hostname) // only works with root
{
    int len = 0, i;
    char data[512];
    data[0] = '\x05';

    while (hostname[len] != '\0')
        len++;
    for (i = 0; i < len; i++)
        data[1 + i] = hostname[i];

    data[1 + len++] = '\0';
    syscall(MYSYSCALL_ID, data, len + 1);
}

void Syscall_GetSysinfo(struct new_utsname *result)
{
    char data[512];
    data[0] = '\x06';
    syscall(MYSYSCALL_ID, data, 1);
    if (result != NULL)
        memcpy(result, data, sizeof(struct new_utsname));
}
