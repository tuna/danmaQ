cmake_minimum_required(VERSION 2.8.11)

option(ENABLE_TEST "Enable test program build" Off)

set(VERSION "0.2.2")

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


if (APPLE)
    add_custom_command(
            COMMAND mkdir -p danmaQ.app/Contents/MacOS
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/macOS/Info.plist danmaQ.app/Contents/Info.plist
            COMMAND cp src/danmaQ danmaQ.app/Contents/MacOS/
            COMMAND sed -i.bak 's/VERSION/${VERSION}/g' danmaQ.app/Contents/Info.plist
            COMMAND mkdir -p Icon.iconset
            COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/src/icons/statusicon.png icon.png
            COMMAND sips -z 16 16   icon.png --out Icon.iconset/icon_16x16.png
            COMMAND sips -z 32 32   icon.png --out Icon.iconset/icon_16x16@2x.png
            COMMAND sips -z 32 32   icon.png --out Icon.iconset/icon_32x32.png
            COMMAND sips -z 64 64   icon.png --out Icon.iconset/icon_32x32@2x.png
            COMMAND sips -z 128 128 icon.png --out Icon.iconset/icon_128x128.png
            COMMAND sips -z 256 256 icon.png --out Icon.iconset/icon_128x128@2x.png
            COMMAND sips -z 256 256 icon.png --out Icon.iconset/icon_256x256.png
            COMMAND sips -z 512 512 icon.png --out Icon.iconset/icon_256x256@2x.png
            COMMAND sips -z 512 512 icon.png --out Icon.iconset/icon_512x512.png
            COMMAND sips -z 1024 1024 icon.png --out Icon.iconset/icon_512x512@2x.png
            COMMAND iconutil -c icns Icon.iconset
            COMMAND mkdir -p danmaQ.app/Contents/Resources
            COMMAND cp Icon.icns danmaQ.app/Contents/Resources/
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/src/danmaQ
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/danmaQ.app
    )
    add_custom_command(
            COMMAND /usr/local/opt/qt/bin/macdeployqt danmaQ.app -dmg
            DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/danmaQ.app
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/danmaQ.dmg
    )
    add_custom_target(app ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/danmaQ.app)
    add_custom_target(dmg ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/danmaQ.dmg)
endif ()

add_subdirectory(src)


if (ENABLE_TEST)
enable_testing()
add_subdirectory(tests)
endif (ENABLE_TEST)
