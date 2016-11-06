cmake_minimum_required(VERSION 2.8.11)

option(ENABLE_TEST "Enable test program build" Off)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5X11Extras)

set(CMAKE_CXX_FLAGS "-Wall --std=c++11 ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)


if (ENABLE_TEST)
enable_testing()
add_subdirectory(tests)
endif (ENABLE_TEST)
