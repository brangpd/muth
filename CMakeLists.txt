cmake_minimum_required(VERSION 3.21)
project(muth)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

add_library(muth SHARED library.cpp library.c)
