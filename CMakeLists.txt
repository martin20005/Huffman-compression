cmake_minimum_required(VERSION 3.15)
project(huffman_1_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(huffman_1_2 main.cpp
        bitbuffer.cpp
        huffman.cpp
        memtrace.cpp
        node.cpp)