set(danmaQ_SOURCES 
	main.cpp 
	danmaku_ui.cpp
	danmaku_window.cpp
	danmaQ_app.cpp
	subscriber.cpp
	)

IF (WIN32 AND NOT MINGW AND NOT CYGWIN)
	SET(danmaQ_SOURCES ${danmaQ_SOURCES} danmaQ.rc)
ENDIF()

set(danmaQ_HEADERS
	subscriber.h
	danmaku_ui.h
	danmaku_window.h
	danmaQ_app.h
	danmaku.h
	)

add_subdirectory(translations)

set(danmaQ_RESOURCES icons.qrc i18n.qrc)

set(CMAKE_AUTOMOC ON)

qt5_add_resources(danmaQ_RCC_SRCS ${danmaQ_RESOURCES})

add_executable(danmaQ ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC} ${danmaQ_RCC_SRCS})
add_dependencies(danmaQ i18n)

set(DANMAQ_LIBRARIES Qt5::Widgets Qt5::Core Qt5::Network)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_package(Qt5X11Extras REQUIRED)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} Qt5::X11Extras)

	find_package(X11 REQUIRED)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X11_LIBRARIES})

	# find_library(X_LIB X11 Xext)
	# set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X_LIB})
endif()

target_link_libraries(danmaQ ${DANMAQ_LIBRARIES})
install(TARGETS danmaQ DESTINATION bin)
