cmake_minimum_required(VERSION 3.10)
project(shell C)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Cannot compile in 32 bit or less architectures")
endif()

set(SOURCES
    main.c 
    includes/utils.c
)

include_directories(includes)

add_executable(C-calc ${SOURCES})

if(WIN32)
    if(MSVC)
        target_compile_definitions(C-calc PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
else()
    target_link_libraries(C-calc m)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(C-calc PRIVATE -Wall -Wextra -Wpedantic)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(C-calc PRIVATE /W4)
endif()
