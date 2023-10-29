cmake_minimum_required(VERSION 3.11.0)

project(6502Cpu C)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

set(source src/cpu.c src/instructions.c src/addressmodes.c)
set(shared_lib ${PROJECT_NAME})
set(static_lib ${PROJECT_NAME}static)

add_library(cpu_object OBJECT ${source})
target_include_directories(cpu_object PRIVATE include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    target_compile_options(cpu_object PRIVATE -Wall -Wextra -ggdb)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") 

elseif(NOT CMAKE_BUILD_TYPE) 
    
endif()

add_library(${shared_lib} SHARED)
target_link_libraries(${shared_lib} PUBLIC cpu_object)

add_library(${static_lib} STATIC)
target_link_libraries(${static_lib} PUBLIC cpu_object)


install(DIRECTORY include/ 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPU6502/Internal
        COMPONENT headers
        FILES_MATCHING 
        PATTERN "*.h" 
        PATTERN  "cpu.h" EXCLUDE)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPU6502/
        COMPONENT headers
        FILES_MATCHING 
        PATTERN "cpu.h")

install(TARGETS ${shared_lib} 
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT shared)

install(TARGETS ${static_lib} 
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT static)
