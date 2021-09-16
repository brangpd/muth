cmake_minimum_required(VERSION 3.16)
project(muth)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

find_package(GTest CONFIG REQUIRED)

add_library(muth SHARED library.c)

add_executable(test test.cpp)
target_link_libraries(test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
