cmake_minimum_required(VERSION 3.17)
project(HashTable)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

add_executable(HashTable main.cpp)