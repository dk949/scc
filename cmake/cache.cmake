include(${CMAKE_SOURCE_DIR}/cmake/misc.cmake)

function (_enable_cache)
    if (NOT ENABLE_CACHE)
        return()
    endif ()

    set(CACHE_PROGRAM
        "ccache"
        CACHE STRING "Compiler cache to be used"
    )

    find_program(_cahce_bin ${CACHE_PROGRAM})
    if (_cahce_bin)
        message(STATUS "${CACHE_PROGRAM} found and enabled")
        set(CMAKE_CXX_COMPILER_LAUNCHER ${_cahce_bin})
    else ()
        message(
            WARNING
                "${CACHE_PROGRAM} is enabled but was not found. Not using it"
        )
    endif ()
endfunction ()

_enable_cache()
unset_function(_enable_cache)
