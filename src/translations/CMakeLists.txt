set(danmaQ_I18N_SRC
    zh_CN.ts
    zh_TW.ts
    ja_JP.ts)

find_program(QT_LRELEASE_EXECUTABLE NAMES lrelease-qt5 lrelease)
if (NOT QT_LRELEASE_EXECUTABLE)
    message(FATAL_ERROR "lrelease not found in PATH")
endif()
set(danmaQ_I18N_FILES)
foreach(I18N_SRC ${danmaQ_I18N_SRC})
    string(REPLACE ".ts" ".qm" I18N_FILE ${I18N_SRC})
    add_custom_command(
        COMMAND ${QT_LRELEASE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_SRC}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_SRC}
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_FILE})
    list(APPEND danmaQ_I18N_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_FILE})
endforeach()
add_custom_target(translations ALL DEPENDS ${danmaQ_I18N_FILES})
