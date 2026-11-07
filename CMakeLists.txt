cmake_minimum_required(VERSION 4.3)

project(gbemu)

add_executable(gbemu)
target_sources(gbemu 
PRIVATE 
    src/main.cpp
)


add_library(cpu)

target_sources(cpu
PRIVATE 
    src/cpu/cpu.cpp
    src/cpu/cu.cpp
    src/cpu/cu_opcodes/opcodes0b00.cpp
    src/cpu/cu_opcodes/opcodes0b11.cpp
    src/cpu/alu.cpp
    src/cpu/idu.cpp
    src/cpu/rf.cpp

PRIVATE
    FILE_SET cpu_private_headers
    TYPE HEADERS 
    BASE_DIRS
        include 
    FILES 
    include/cpu/cu.h
    include/cpu/alu.h
    include/cpu/idu.h
    include/cpu/rf.h

PUBLIC
    FILE_SET cpu_public_headers
    TYPE HEADERS 
    BASE_DIRS
        include
    FILES
        include/cpu/cpu.h
)


add_library(busses)

target_sources(busses 
PRIVATE
    src/bus/address.cpp
    src/bus/data.cpp
    src/bus/controll.cpp
PUBLIC
    FILE_SET busses
    TYPE HEADERS
    BASE_DIRS
        include
    FILES 
    include/bus/address.h
    include/bus/data.h
    include/bus/controll.h
)


add_library(memory)

target_sources(memory
PRIVATE 
    src/memory/memory.cpp
PUBLIC
    FILE_SET memory_headers
    TYPE HEADERS 
    BASE_DIRS
        include
    FILES 
        include/memory/memory.h
)


target_link_libraries(cpu 
PRIVATE 
    busses
)

target_link_libraries(memory 
PRIVATE 
    busses
)


target_link_libraries(gbemu 
PRIVATE 
    cpu
    memory
)



find_package(GTest REQUIRED)
add_executable(test)
target_sources(test 
PRIVATE
    test/cpu.cpp
    test/cpu_0x40-0x7f.cpp
    test/cpu_0x80-0xbf.cpp
)
target_link_libraries(test GTest::gtest GTest::gtest_main
    cpu
)