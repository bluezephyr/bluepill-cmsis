cmake_minimum_required(VERSION 3.15)

# Project settings
project(bluepill-cmsis C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Use the cross-compile version of add_executable defined in the toolchain
cc_add_executable(ledblink src/main.c)
