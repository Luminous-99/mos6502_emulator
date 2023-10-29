cmake_minimum_required(VERSION 3.11.0)

project(6502Cpu C)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

set(source src/cpu.c src/instructions.c src/addressmodes.c)
set(shared_lib ${PROJECT_NAME})
set(static_lib ${PROJECT_NAME}static)

add_library(${shared_lib} SHARED ${source})
add_library(${static_lib} STATIC ${source})


target_include_directories(${shared_lib} PRIVATE include/)
target_include_directories(${static_lib} PRIVATE include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -ggdb)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") 

elseif(NOT CMAKE_BUILD_TYPE) 
    
endif()

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
        COMPONENT lib)

install(TARGETS ${static_lib}static 
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT lib)
