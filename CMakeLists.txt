cmake_minimum_required(VERSION 2.8.11)

PROJECT(danmaQ)

option(ENABLE_TEST "Enable test program build" Off)

set(VERSION "0.2.3.1")

set(CMAKE_CXX_STANDARD 14)

IF (WIN32 AND NOT MINGW AND NOT CYGWIN)
	set(CMAKE_PREFIX_PATH "$ENV{QTDIR} ${CMAKE_PREFIX_PATH}")
ENDIF()

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
        set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
ENDIF ()

IF (WIN32 AND NOT MINGW AND NOT CYGWIN)
	set(CMAKE_CXX_FLAGS "/permissive- /MP ${CMAKE_CXX_FLAGS}")
	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
ENDIF()

if (APPLE)
    add_custom_command(
            COMMAND mkdir -p Icon.iconset
            COMMAND cp ${CMAKE_SOURCE_DIR}/src/icons/statusicon.png icon.png
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
            COMMAND mkdir -p ${CMAKE_PROJECT_NAME}.app/Contents/Resources
            DEPENDS ${CMAKE_SOURCE_DIR}/src/icons/statusicon.png
            OUTPUT ${CMAKE_BINARY_DIR}/Icon.icns
    )
    add_custom_command(
            COMMAND mkdir -p ${CMAKE_PROJECT_NAME}.app/Contents/MacOS
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/macOS/Info.plist ${CMAKE_PROJECT_NAME}.app/Contents/Info.plist
            COMMAND cp src/${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.app/Contents/MacOS/
            COMMAND sed -i.bak 's/VERSION/${VERSION}/g' ${CMAKE_PROJECT_NAME}.app/Contents/Info.plist
            COMMAND cp Icon.icns ${CMAKE_PROJECT_NAME}.app/Contents/Resources/
            COMMAND /usr/local/opt/qt/bin/macdeployqt ${CMAKE_PROJECT_NAME}.app -no-strip
            DEPENDS ${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}
            DEPENDS ${CMAKE_BINARY_DIR}/Icon.icns
            OUTPUT ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app
    )
    add_custom_command(
            COMMAND cp -r ${CMAKE_PROJECT_NAME}.app ${CMAKE_PROJECT_NAME}.app.bak
            COMMAND /usr/local/opt/qt/bin/macdeployqt ${CMAKE_PROJECT_NAME}.app -dmg
            COMMAND rm -rf ${CMAKE_PROJECT_NAME}.app
            COMMAND mv ${CMAKE_PROJECT_NAME}.app.bak ${CMAKE_PROJECT_NAME}.app
            DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app
            OUTPUT ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.dmg
    )
    add_custom_target(app ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.app)
    add_custom_target(dmg DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.dmg)
endif ()

add_subdirectory(src)
add_subdirectory(man)


if (ENABLE_TEST)
enable_testing()
add_subdirectory(tests)
endif (ENABLE_TEST)
