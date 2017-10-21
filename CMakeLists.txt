cmake_minimum_required(VERSION 2.8.11)

option(ENABLE_TEST "Enable test program build" Off)

# From https://github.com/Homebrew/homebrew-core/issues/8392
if(APPLE AND EXISTS /usr/local/opt/qt)
	# Because Qt5 is key-only, Homebrew installs Qt5 in
	# /usr/local/qt5, ensure it can be found by CMake since
	# it is not in the default /usr/local prefix.
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

IF (UNIX)
        set(CMAKE_CXX_FLAGS "-Wall --std=c++11 ${CMAKE_CXX_FLAGS}")
ENDIF ()


add_subdirectory(src)


if (ENABLE_TEST)
enable_testing()
add_subdirectory(tests)
endif (ENABLE_TEST)
