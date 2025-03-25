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

qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES zh_CN zh_TW ja_JP)

set(CMAKE_AUTOMOC ON) 
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON)

add_subdirectory(resource)

set(danmaQ_RESOURCES icons.qrc socket.qrc)
file(GLOB danmqQ_TRANSLATIONS translations/*.ts)

qt_add_resources(danmaQ_RCC_SRCS ${danmaQ_RESOURCES})
qt_add_executable(${CMAKE_PROJECT_NAME} ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC} ${danmaQ_RCC_SRCS})
qt_add_translations(${CMAKE_PROJECT_NAME}
    TS_FILES ${danmqQ_TRANSLATIONS}
)

set(DANMAQ_LIBRARIES Qt6::Widgets Qt6::Core Qt6::Network Qt6::WebEngineWidgets)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${DANMAQ_LIBRARIES})
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)
