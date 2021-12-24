cmake_minimum_required(VERSION 2.8.12)

project(MyFatFS)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/disk
)

file(GLOB SRC_COMMON "${PROJECT_SOURCE_DIR}/common/*.cpp" "${PROJECT_SOURCE_DIR}/common/*.h")
file(GLOB SRC_DISK "${PROJECT_SOURCE_DIR}/disk/*.cpp" "${PROJECT_SOURCE_DIR}/disk/*.h")

set(SRC_MAIN ./main.cpp ${SRC_COMMON} ${SRC_DISK})

add_executable(${PROJECT_NAME} ${SRC_MAIN})

ADD_SUBDIRECTORY(disk ./disk)
