#include <stdio.h>
#include "mysyscall.h"

int main()
{
    int pid;
    int nice, prio;
    int oldNice, oldPrio, newNice, newPrio;
    char hostname[512];
    printf("target pid:\n");
    scanf("%d", &pid);
    Syscall_GetNice(pid, &nice, &prio);
    printf("nice: %d, prio: %d\n", nice, prio);
    printf("new nice:\n");
    scanf("%d", &nice);
    Syscall_SetNice(pid, nice, &oldNice, &oldPrio, &newNice, &newPrio);
    printf("oldNice: %d, oldPrio: %d, newNice: %d, newPrio: %d\n", oldNice, oldPrio, newNice, newPrio);
    return 0;
}
