set(danmaQ_SOURCES 
	main.cpp 
	danmaku_ui.cpp
	danmaku_window.cpp
	)

set(danmaQ_HEADERS
	danmaku_ui.h
	danmaku_window.h
	danmaku.h
	)


Qt4_wrap_cpp(danmaQ_HEADERS_MOC ${danmaQ_HEADERS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(danmaQ ${danmaQ_SOURCES} ${danmaQ_HEADERS_MOC})

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
	find_library(X_LIB Xext)
	target_link_libraries(danmaQ ${QT_LIBRARIES} ${X_LIB})
else()
	target_link_libraries(danmaQ ${QT_LIBRARIES})
endif()
