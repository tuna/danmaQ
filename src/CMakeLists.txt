set(danmaQ_SOURCES 
	main.cpp 
	danmaku_ui.cpp
	danmaku_window.cpp
	subscriber.cpp
	)

set(danmaQ_HEADERS
	subscriber.h
	danmaku_ui.h
	danmaku_window.h
	danmaku.h
	)

set(CMAKE_AUTOMOC ON)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

find_package(QJSON REQUIRED)

add_executable(danmaQ ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_library(X_LIB Xext)
	target_link_libraries(danmaQ ${QT_LIBRARIES} ${QJSON_LIBRARIES} ${X_LIB})
else()
	target_link_libraries(danmaQ ${QT_LIBRARIES} ${QJSON_LIBRARIES})
endif()
