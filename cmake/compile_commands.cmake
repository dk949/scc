function (link_compile_commands)
    if (MSVC OR NOT CMAKE_EXPORT_COMPILE_COMMANDS)
        return()
    endif ()
    set(_current_list_path ${CMAKE_CURRENT_LIST_DIR})

    string(
        REGEX
        REPLACE "\/|\\|\ "
                "_"
                _dir
                ${_current_list_path}
    )

    add_custom_target(
        ${_dir}_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json
                ${_current_list_path}/compile_commands.json
    )

endfunction ()
