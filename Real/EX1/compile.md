# Compile config
```
Power management and ACPI options->
ACPI (Advanced Configuration and Power Interface) Support[x]

Networking support[x](reduce a lot of time)

Device Drivers->
GNSS Receiver support[x]
Parallel port support[x]
Remote Controller support[x]
Multimedia support[x]
Sound card support[x]
Staging drivers[x]
Platform support for Chrome hardware (transitional)[x]
Platform support for Chrome hardware[x]
Platform support for Mellanox hardware[x]
Industrial I/O support[x]
Thunderbolt support[x]
Android->Android Drivers[x]
FPGA Configuration Framework[x]
Graphics support->
Direct Rendering Manager[x](reduce a lot of time)
DRM Support for Xen guest OS[x]

File systems->
Reiserfs support[x]
JFS support[x]
XFS support[x]
GFS2 support[x]
OCFS2 support[x]
BtrFS support[x]
NILFS2 support[x]
F2FS support[x]

Cryptographic API->
Certificates for signature checking->
Provide system-wide ring of trusted keys->
Additional X.509 keys for default system keyring(clear)
```

# Edited files(patch)

linux-5.4.155/arch/x86/entry/syscalls/syscall_64.tbl  
linux-5.4.155/include/linux/syscalls.h  
linux-5.4.155/kernel/sys.c  

# tips
make -j5  
make install  
make modules_install  
update-initramfs  
update-grub  