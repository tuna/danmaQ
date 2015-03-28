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
set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

find_package(QJSON REQUIRED)

add_executable(danmaQ ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC})

set(DANMAQ_LIBRARIES ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${QJSON_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_library(X_LIB Xext)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X_LIB})
endif()

target_link_libraries(danmaQ ${DANMAQ_LIBRARIES})
