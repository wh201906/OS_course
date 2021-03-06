# 操作系统实验

杭电2021年操作系统实验  
[English](README.md)  

## 实验一
内核修改时有自己标签，用 dmesg 验证  
+ Linux 内核标签（系统启动显示一次）
+ 显示当前系统名称和版本的系统调用（内核、用户都有显示）
+ 修改进程nice和prio值的系统调用（内核、用户都有显示）
+ 改变主机名称为自定义字符串的系统调用（内核、用户都有显示）

## 实验二
设计内核模块
+ 设计一个模块，要求列出系统中所有内核线程的程序名、PID 、进程状态、进程优先级、父进程的 PID  
    输出按列对齐  
+ 设计一个带参数的模块,其参数为某个进程的 PID 号，模块的功能是列出该进程的家族信息，包括父进程、兄弟进程和子进程的程序名、PID 号及进程状态  
    输出树结构，类似 pstree 的输出


## 实验三
Linux进程管理  
+ 实现一个模拟的shell  
    编写三个不同的程序 cmd1.c, cmd2.c及cmd3.c，每个程序的功能自定，分别编译成可执行文件 cmd1 、cmd2 及 cmd3 。然后再编写一个程序，模拟 shell 程序的功能：能根据用输入的字符串(表示相应的命令名)，为相应的命令创建子进程并让它去执行相应的程序，而父进程则等待子进程结束，然后再等待接收下一条命令。如果接收到的命令为 exit ，则父进程结束。如果接收到的命令是无效命令，则显示 “Command not found” ，继续等待输入下一条命令  
    此外，还要支持调用find, grep命令  
    未要求实现管道
+ 实现一个管道通信程序  
    由父进程创建一个管道，然后再创建三个子进程，并由这三个子进程利用管道与父进程之间进行通信：子进程发送信息，父进程等三个子进程全部发完消息后再接收信息。通信的具体内容可根据自己的需要随意设计，要求能试验阻塞型读写过程中的各种情况，测试管道的默认大小，并且要求利用 Posix 信号量机制实现进程间对管道的互斥访问。运行程序，观察各种情况下，进程实际读写的字节数以及进程阻塞唤醒的情况  
    此外，还要实现有名管道通信（独立进程, FIFO）  
+ 利用 linux的消息队列通信机制实现两个线程间的通信  
    实现多进程间同步、互斥通信  
+ 利用 linux的共享内存通信机制实现两个进程间的通信  
    编写程序 sender，它创建一个共享内存，然后等待用户通过终端输入一串字符，并将这串字符通过共享内存发送给 receiver；最后,它等待 receiver 的应答，收到应答消息后，将接收到的应答信息显示在终端屏幕上，删除共享内存，结束程序的运行。编写 receiver 程序，它通过共享内存接收来自 sender 的消息，将消息显示在终端屏幕上，然后再通过该共享内存向 sender 发送一个应答消息 “over”，结束程序的运行。选择合适的信号量机制实现两个进程对共享内存的互斥及同步使用  
    此外，还要实现双向通信  

## 实验四
+ 在内存中开辟一个虚拟磁盘空间作为文件存储分区，在其上实现一个简单的基于多级目录的单用户单任务系统中的文件系统。在退出该文件系统的使用时，应将虚拟磁盘上的内容以一个文件的方式保存到磁盘上，以便下次可以再将它恢复到内存的虚拟磁盘中。
+ 文件物理结构可采用显式链接或其他结构。
+ 空闲磁盘空间的管理可选择 FAT 表、位示图或其他办法。
+ 文件目录结构采用多级目录结构。为简单起见，可以不使用索引结点，每个目录项应包含文件名、物理地址、长度等信息，还可以通过目录项实现对文件的读和写的保护。
+ 此外，还要求实现以下功能  
    打印FAT数据（按16进制格式显示，每行16项）  
    实现顺序、随机读写  
    实现写入目录、文件多于1个扇区数据信息  
    扇区大小可变 （扇区字节=1024～64）  