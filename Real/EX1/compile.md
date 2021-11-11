# Compile config
```
make localmodconfig
```
this saves a lot of time  
# Edited files(patch)

linux-5.4.155/arch/x86/entry/syscalls/syscall_64.tbl  
linux-5.4.155/include/linux/syscalls.h  
linux-5.4.155/kernel/sys.c  
linux-5.4.155/init/main.c

# tips
make -j5  
make install  
make modules_install  
update-initramfs -u  
update-grub  