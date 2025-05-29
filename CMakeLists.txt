include(FetchContent)

cmake_minimum_required(VERSION 3.30)
project(string_builder C)

set(CMAKE_C_STANDARD 11)

add_library(string_builder STATIC string_builder.c string_builder.h)

if(NOT FLUENT_LIBC_RELEASE) # Manually add libraries only if not in release mode
    FetchContent_Declare(
            types
            GIT_REPOSITORY https://github.com/rodrigoo-r/types
            GIT_TAG        master
    )

    FetchContent_MakeAvailable(types)

    target_include_directories(string_builder PRIVATE ${CMAKE_BINARY_DIR}/_deps/types-src)
    target_link_libraries(string_builder PRIVATE types)
endif ()
