#include <stdio.h>
#include "mysyscall.h"

int main()
{
    char hostname[512];
    Syscall_GetHostname(hostname);
    printf("old hostname: %s\n", hostname);
    printf("new hostname:\n");
    scanf("%s", hostname);
    Syscall_SetHostname(hostname);
    Syscall_GetHostname(hostname);
    printf("current hostname: %s\n", hostname);
    return 0;
}
