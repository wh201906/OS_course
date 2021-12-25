cmake_minimum_required(VERSION 2.8.12)

project(MyFatFS)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/disk
    ${PROJECT_SOURCE_DIR}/fat32
)

file(GLOB SRC_COMMON "${PROJECT_SOURCE_DIR}/common/*.cpp" "${PROJECT_SOURCE_DIR}/common/*.h")
file(GLOB SRC_DISK "${PROJECT_SOURCE_DIR}/disk/*.cpp" "${PROJECT_SOURCE_DIR}/disk/*.h")
file(GLOB SRC_FAT32 "${PROJECT_SOURCE_DIR}/fat32/*.cpp" "${PROJECT_SOURCE_DIR}/fat32/*.h")

set(SRC_MAIN ./main.cpp ${SRC_COMMON} ${SRC_DISK} ${SRC_FAT32})

add_executable(${PROJECT_NAME} ${SRC_MAIN})

ADD_SUBDIRECTORY(common ./common)
ADD_SUBDIRECTORY(disk ./disk)
ADD_SUBDIRECTORY(fat32 ./fat32)
