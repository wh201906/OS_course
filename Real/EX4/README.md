# FAT32

简易FAT32文件系统实现，仅支持短文件名。  
实现功能模块：  
+ 磁盘(不支持扩展分区)  
    + MBR(主引导记录)  
        启动代码, DPT, 55AA标记  
    + DPT(分区表)  
        是否为活动分区，起始CHS/LBA, 文件系统类型，终止CHS，分区大小  
+ FAT32分区  
    + DBR(DOS Boot Record, DOS引导记录)  
        跳转代码，OEM名称，BPB，启动代码，55AA标记  
    + BPB(BIOS Parameter Block, BIOS参数块)  
        若干FAT32分区的参数，数据分配示意图  
    + FSInfo(文件系统信息)  
        FSInfo标志，空闲簇数量，下一个空闲簇位置，55AA标记  
    + FAT表  
        表项，簇数量，空闲簇数量，下一个空闲簇位置  
    + DEntry(目录项)  
        名称，扩展名，属性，簇ID，大小，重命名，提取完整名称  
    + Datacluster(数据簇)  
        簇ID转换为簇地址  
+ 访问器  
    + 目录访问器  
        ls, cd, pwd, find, mkdir, mkfile, rename, remove, move  
    + 文件访问器  
        open, read, write, resize  