cmake_minimum_required(VERSION 2.8.12)

project(MyFatFS)

file(GLOB SRC_FILES "*.cpp" "*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES})

