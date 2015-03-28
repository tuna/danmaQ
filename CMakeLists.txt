cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED)
set(CMAKE_CXX_FLAGS "-Wall --std=c++11 ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)
