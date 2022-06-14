function (enable_sanitizers project_name)
    if (NOT ENABLE_SANITIZERS)
        return()
    endif ()

    if (MSVC)
        set(SANITIZER_LIST "address")
        set(_cli_prefix "/")
    else ()
        set(_cli_prefix "-")
    endif ()

    message(STATUS "Running with sanitizers: [${SANITIZER_LIST}]")
    target_compile_options(
        ${project_name} PRIVATE ${_cli_prefix}fsanitize=${SANITIZER_LIST}
    )
    target_link_options(
        ${project_name} PRIVATE ${_cli_prefix}fsanitize=${SANITIZER_LIST}
    )
endfunction ()
