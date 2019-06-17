cmake_minimum_required(VERSION 3.12)
project(BufferManager)

set(CMAKE_CXX_STANDARD 14)

add_executable(BufferManager ./src/main.cpp ./src/BufferManager.cpp)