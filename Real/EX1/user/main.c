#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysyscall.h"
int main()
{
    int pid;
    printf("target pid:\n");
    scanf("%d", &pid);
    int nice, prio;
    int oldNice, oldPrio, newNice, newPrio;
    char hostname[512];
    struct new_utsname sysinfo;
    Syscall_Test();
    Syscall_Show("Hello\n");
    Syscall_GetNice(pid, &nice, &prio);
    printf("nice: %d, prio: %d\n", nice, prio);
    Syscall_SetNice(pid, 1, &oldNice, &oldPrio, &newNice, &newPrio);
    printf("oldNice: %d, oldPrio: %d, newNice: %d, newPrio: %d\n", oldNice, oldPrio, newNice, newPrio);
    Syscall_GetHostname(hostname);
    printf("hostname: %s\n", hostname);
    strcpy(hostname, "newname");
    Syscall_SetHostname(hostname); // only works with root
    Syscall_GetHostname(hostname);
    printf("hostname: %s\n", hostname);
    Syscall_GetSysinfo(&sysinfo);
    printf("System Name: %s, Node Name: %s, Release: %s, Version: %s, Machine: %s\n", sysinfo.sysname, sysinfo.nodename, sysinfo.release, sysinfo.version, sysinfo.machine);
    // system("dmesg");
    return 0;
}