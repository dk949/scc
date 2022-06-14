# cmake-format: off


# Targets
set(EXE_NAME ${CMAKE_PROJECT_NAME} CACHE STRING "Name of the main executable")
set(DOXYFILE_SUFFIX ${CMAKE_PROJECT_NAME}_docs CACHE STRING "Documentation target")



# Artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "archive location")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE STRING "library location")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "executable location")



# Analysers
option(ENABLE_CPPCHECK "Enable cppcheck" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable include-what-you-use" ON)



# Sanitizers
option(ENABLE_SANITIZERS "Enable sanitizers" ON)
set(SANITIZER_LIST "address,leak,undefined" CACHE STRING "List of sanitizers to use")



# Docs
option(ENABLE_DOXYGEN "Enable doxygen doc builds of source" ON)



# Warnings
option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)



# Caching
option(ENABLE_CACHE "Enable cache if available" ON)



# Language standard and extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)



# Compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# cmake-format: on
