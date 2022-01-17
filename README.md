# OS coursework

Some experiments of HDU's 2021 Operating Systems course  
[中文](README_zh_CN.md)  

## EX1  
Modify the kernel code then build it  
+ Show a message in the kernel ring buffer when booting up  
+ Add a system call to show the system information(from utsname)  
+ Add a system call to get/set the nice value of a process  
+ Add a system call to get/set the hostname  

## EX2  
Make some kernel modules  
+ Show the process status  
+ Show the tree of a process  

## EX3  
Test inter-process communication  
+ Make a shell which forks itself to execute the commands  
+ Test pipe with mutex/semaphore  
+ Test message queue with mutex/semaphore  
+ Test shared memory with mutex/semaphore  

## EX4  
Write a filesystem  