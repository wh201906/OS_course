#include <stdio.h>
#include "mysyscall.h"

int main()
{
    int pid;
    int nice, prio;
    int oldNice, oldPrio, newNice, newPrio;
    char hostname[512];
    struct new_utsname sysinfo;
    Syscall_GetSysinfo(&sysinfo);
    printf("System Name: %s, Node Name: %s, Release: %s, Version: %s, Machine: %s\n", sysinfo.sysname, sysinfo.nodename, sysinfo.release, sysinfo.version, sysinfo.machine);
    return 0;
}
