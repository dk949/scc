include(${CMAKE_SOURCE_DIR}/cmake/misc.cmake)

macro (_enable_stat _stat)
    string(TOUPPER ${_stat} _STAT)
    string(
        REPLACE "-"
                "_"
                _STAT
                ${_STAT}
    )
    if (ENABLE_${_STAT})
        find_program(_${_STAT} ${_stat})
        if (_${_STAT})
            set(CMAKE_CXX_${_STAT} ${_${_STAT}} ${ARGN})
        else ()
            message(WARNING "${_stat} requested but executable not found")
        endif ()
    endif ()
endmacro ()

_enable_stat(
    cppcheck
    --suppress=missingInclude
    --enable=all
    --inline-suppr
    --inconclusive
)
_enable_stat(clang-tidy -extra-arg=-Wno-unknown-warning-option)
_enable_stat(include-what-you-use)

unset_function(_enable_stat)
