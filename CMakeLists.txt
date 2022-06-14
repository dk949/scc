cmake_minimum_required(VERSION 3.18)
include(cmake/setup_pkg.cmake)
project(
    ${THIS_PROJECT_NAME}
    VERSION ${THIS_PROJECT_VERSION}
    DESCRIPTION ${THIS_PROJECT_DESCRIPTION}
    HOMEPAGE_URL "https://github.com/dk949/cpp-init"
    LANGUAGES CXX
)

include(cmake/options.cmake)

include(cmake/static_analyzers.cmake)
include(cmake/cache.cmake)
include(cmake/compile_commands.cmake)
include(cmake/compiler_warnings.cmake)
include(cmake/doxygen.cmake)
include(cmake/sanitizers.cmake)

add_subdirectory(include)
add_subdirectory(test)
add_subdirectory(project_config)
