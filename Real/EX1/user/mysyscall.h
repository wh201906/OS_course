#ifndef _OS_MYSYSCALL_H_
#define _OS_MYSYSCALL_H_

#define MYSYSCALL_ID 436
#define __NEW_UTS_LEN 64

struct new_utsname {
	char sysname[__NEW_UTS_LEN + 1];
	char nodename[__NEW_UTS_LEN + 1];
	char release[__NEW_UTS_LEN + 1];
	char version[__NEW_UTS_LEN + 1];
	char machine[__NEW_UTS_LEN + 1];
	char domainname[__NEW_UTS_LEN + 1];
};

void Syscall_Test();
void Syscall_Show(char *str);
void Syscall_GetNice(int pid, int *nice, int *prio);
void Syscall_SetNice(int pid, int nice, int *oldNice, int *oldPrio, int *newNice, int *newPrio);
void Syscall_GetHostname(char *result);
void Syscall_SetHostname(char *hostname); // only works with root
void Syscall_GetSysinfo(struct new_utsname *result);

#endif