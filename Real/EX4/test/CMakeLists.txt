add_executable(t_datahandle ./datahandle.cpp ${SRC_DISK} ${SRC_COMMON} ${SRC_FAT32})
add_executable(t_format ./format.cpp ${SRC_DISK} ${SRC_COMMON} ${SRC_FAT32})
add_executable(t_info ./info.cpp ${SRC_DISK} ${SRC_COMMON} ${SRC_FAT32})
add_executable(t_showhex ./showhex.cpp ${SRC_DISK} ${SRC_COMMON} ${SRC_FAT32})