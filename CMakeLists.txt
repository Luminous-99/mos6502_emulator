cmake_minimum_required(VERSION 3.11.0)

project(6502Cpu C)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

set(source src/cpu.c src/instructions.c src/addressmodes.c)

add_library(${PROJECT_NAME} SHARED ${source})

target_include_directories(${PROJECT_NAME} PRIVATE include/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -ggdb)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") 

elseif(NOT CMAKE_BUILD_TYPE) 
    
endif()

install(DIRECTORY include/. 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPU6502/Internal
        FILES_MATCHING 
        PATTERN "*.h" 
        PATTERN  "cpu.h" EXCLUDE)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/cpu.h)
install(TARGETS ${PROJECT_NAME} 
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CPU6502)

configure_file(pkg-config/6502Cpu.pc.in 6502Cpu.pc @ONLY)

install(FILES ${CMAKE_BINARY_DIR}/6502Cpu.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
