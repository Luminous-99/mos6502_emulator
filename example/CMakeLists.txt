cmake_minimum_required(VERSION 3.11)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

project(6502Cpu)

set(source ./main.c ../src/cpu.c ../src/instructions.c ../src/addressmodes.c)

add_executable(6502Cpu ${source})

target_include_directories(6502Cpu PRIVATE "../include")

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    set(CFLAGS " -Wall -Wextra -ggdb")
    set(CMAKE_C_FLAGS "${CFLAGS}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") 
endif()
