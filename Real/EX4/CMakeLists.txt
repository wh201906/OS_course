cmake_minimum_required(VERSION 2.8.12)

project(MyFatFS)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/disk
    ${PROJECT_SOURCE_DIR}/fat32
    ${PROJECT_SOURCE_DIR}/access
)

file(GLOB SRC_COMMON "${PROJECT_SOURCE_DIR}/common/*.cpp" "${PROJECT_SOURCE_DIR}/common/*.h")
file(GLOB SRC_DISK "${PROJECT_SOURCE_DIR}/disk/*.cpp" "${PROJECT_SOURCE_DIR}/disk/*.h")
file(GLOB SRC_FAT32 "${PROJECT_SOURCE_DIR}/fat32/*.cpp" "${PROJECT_SOURCE_DIR}/fat32/*.h")
file(GLOB SRC_ACCESS "${PROJECT_SOURCE_DIR}/access/*.cpp" "${PROJECT_SOURCE_DIR}/access/*.h")

set(SRC_MAIN ./main.cpp ${SRC_COMMON} ${SRC_DISK} ${SRC_FAT32} ${SRC_ACCESS})

add_executable(${PROJECT_NAME} ${SRC_MAIN})

ADD_SUBDIRECTORY(test ./test)
