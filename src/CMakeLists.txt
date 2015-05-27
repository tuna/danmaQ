set(danmaQ_SOURCES 
	main.cpp 
	danmaku_ui.cpp
	danmaku_window.cpp
	danmaQ_app.cpp
	subscriber.cpp
	)

set(danmaQ_HEADERS
	subscriber.h
	danmaku_ui.h
	danmaku_window.h
	danmaQ_app.h
	danmaku.h
	)

set(danmaQ_RESOURCES icons.qrc)

set(CMAKE_AUTOMOC ON)
set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

QT4_ADD_RESOURCES(danmaQ_RCC_SRCS ${danmaQ_RESOURCES})

if (WIN32)
	set(CMAKE_PREFIX_PATH "C:/Program Files/qjson/lib/cmake/qjson/")
	find_library (QJSON_LIBRARIES
		"C:/Program Files/qjson/lib"
		${QJSON_LIBRARY_DIRS}
		${LIB_INSTALL_DIR}
	)
	include_directories("C:/Program Files/qjson/include")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
endif(WIN32)

find_package(QJSON REQUIRED)

add_executable(danmaQ ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC} ${danmaQ_RCC_SRCS})

set(DANMAQ_LIBRARIES ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${QJSON_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_library(X_LIB Xext)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X_LIB})
endif()

target_link_libraries(danmaQ ${DANMAQ_LIBRARIES})
install(TARGETS danmaQ DESTINATION bin)
