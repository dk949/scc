link_compile_commands()
find_package(Catch2 CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_path(INCLUDE_DIR "catch.hpp")

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/test/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/test/*.hpp")
file(GLOB_RECURSE LIB_FILES "${CMAKE_SOURCE_DIR}/include/*.hpp")

add_executable(${EXE_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${LIB_FILES})

target_link_libraries(${EXE_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${EXE_NAME} PUBLIC tl::expected)

target_include_directories(
    ${EXE_NAME} PUBLIC "${CMAKE_BINARY_DIR}/project_config/include"
)
target_include_directories(${EXE_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(${EXE_NAME} PUBLIC "${INCLUDE_DIR}")
enable_sanitizers(${EXE_NAME})
set_target_warnings(${EXE_NAME})
