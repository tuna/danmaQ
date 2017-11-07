set(danmaQ_I18N_SRC
    i18n_zh.ts)

find_program(LRELEASE lrelease)
find_program(LUPDATE lupdate)
set(danmaQ_I18N_FILES)
foreach(I18N_SRC ${danmaQ_I18N_SRC})
    string(REPLACE ".ts" ".qm" I18N_FILE ${I18N_SRC})
    add_custom_command(
        COMMAND ${LRELEASE} ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_SRC}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_SRC}
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_FILE})
    list(APPEND danmaQ_I18N_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${I18N_FILE})
endforeach()
add_custom_target(translations ALL DEPENDS ${danmaQ_I18N_FILES})
