cmake_minimum_required(VERSION 3.10)
project(clox)

file(GLOB CORE_SOURCES "src/core/*.c")
file(GLOB DEBUG_SOURCES "src/debug/*.c")
file(GLOB UTILS_SOURCES "src/utils/*.c")

add_executable(clox 
    main.c 
    ${CORE_SOURCES}
    ${DEBUG_SOURCES}
    ${UTILS_SOURCES}
)

target_include_directories(clox PUBLIC src)