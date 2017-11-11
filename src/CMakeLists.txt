set(danmaQ_SOURCES 
	main.cpp 
	Danmaku.cpp
	DMCanvas.cpp
	DMMainWindow.cpp
	Subscriber.cpp
	)

IF (WIN32 AND NOT MINGW AND NOT CYGWIN)
	SET(danmaQ_SOURCES ${danmaQ_SOURCES} danmaQ.rc)
ENDIF()

set(danmaQ_HEADERS
	Subscriber.hpp
	Danmaku.hpp
	DMCanvas.hpp
	DMMainWindow.hpp
	common.h
	)

add_subdirectory(translations)

set(danmaQ_RESOURCES icons.qrc i18n.qrc)

add_custom_target(resources ALL DEPENDS ${danmaQ_RESOURCES})
add_dependencies(resources translations)

set(CMAKE_AUTOMOC ON)

qt5_add_resources(danmaQ_RCC_SRCS ${danmaQ_RESOURCES})

add_executable(${CMAKE_PROJECT_NAME} ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC} ${danmaQ_RCC_SRCS})
add_dependencies(${CMAKE_PROJECT_NAME} resources)

set(DANMAQ_LIBRARIES Qt5::Widgets Qt5::Core Qt5::Network)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_package(Qt5X11Extras REQUIRED)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} Qt5::X11Extras)

	find_package(X11 REQUIRED)
	set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X11_LIBRARIES})

	# find_library(X_LIB X11 Xext)
	# set(DANMAQ_LIBRARIES ${DANMAQ_LIBRARIES} ${X_LIB})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} ${DANMAQ_LIBRARIES})
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
