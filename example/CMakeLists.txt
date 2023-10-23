cmake_minimum_required(VERSION 3.11)

project(cpu_example C)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_library(cpulib NAMES lib6502Cpu.so) 
set(source ./main.c)

add_executable(${PROJECT_NAME} ${source})
target_link_libraries(${PROJECT_NAME} ${cpulib})

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -ggdb)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") 

elseif(NOT CMAKE_BUILD_TYPE) 

endif()
