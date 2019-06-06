cmake_minimum_required(VERSION 3.14)
project(chip_8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-lncurses")

add_executable(chip_8 main.cpp chip8.cpp chip8.h)
add_executable(test chip8.cpp chip8.h tests.cpp)