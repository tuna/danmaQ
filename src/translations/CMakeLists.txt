set(danmaQ_I18N_SRC
    zh_CN.ts
    zh_TW.ts
    ja_JP.ts)

find_package(Qt5LinguistTools)

add_custom_target(update_all_ts_files ALL)
add_custom_target(create_all_qm_files ALL)
add_dependencies(create_all_qm_files update_all_ts_files)

set(danmaQ_I18N_FILES)
foreach(I18N_SRC ${danmaQ_I18N_SRC})
    get_filename_component(I18N_NAME ${I18N_SRC} NAME_WE)
    set(TS_TARGET_NAME "update_ts_file_${I18N_NAME}")
    add_custom_target(${TS_TARGET_NAME}
        COMMAND Qt5::lupdate ${CMAKE_SOURCE_DIR} -ts ${I18N_SRC})
    add_dependencies(update_all_ts_files ${TS_TARGET_NAME})
    set(QM_TARGET_NAME "create_qm_file_${I18N_NAME}")
    set(QM_FILE "${CMAKE_CURRENT_BINARY_DIR}/${I18N_NAME}.qm")
    add_custom_target(${QM_TARGET_NAME}
        COMMAND Qt5::lrelease ${I18N_SRC} -qm ${QM_FILE}
        BYPRODUCTS ${QM_FILE})
    add_dependencies(${QM_TARGET_NAME} ${TS_TARGET_NAME})
    add_dependencies(create_all_qm_files ${QM_TARGET_NAME})
    list(APPEND danmaQ_I18N_FILES ${QM_FILE})
endforeach()
add_custom_target(translations ALL DEPENDS create_all_qm_files ${danmaQ_I18N_FILES})