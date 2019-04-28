cmake_minimum_required(VERSION 3.13)
project(shared_ptr)

set(CMAKE_CXX_STANDARD 14)

add_executable(shared_ptr src/main.cpp src/shared_ptr.h src/unique_ptr.h)