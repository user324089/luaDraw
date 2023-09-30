add_custom_target (script ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_LIST_DIR}/defaultScript.lua
    $<TARGET_FILE_DIR:main>/script.lua
    )
