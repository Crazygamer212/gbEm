cmake_minimum_required(VERSION 4.3)


project(gbemu)

add_executable(gbemu)
target_sources(gbemu 
PUBLIC 
    src/main.cpp
)