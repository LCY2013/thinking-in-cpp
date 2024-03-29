cmake_minimum_required(VERSION 3.22)

set (TARGET Asyncpp)

project(${TARGET})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_custom_target(std_modules ALL
            COMMAND ${CMAKE_COMMAND} -E echo "Building standard library modules"
            COMMAND g++ -fmodules-ts -std=c++20 -c -x c++-system-header iostream cstdint type_traits string filesystem functional coroutine thread chrono mutex vector
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

function (prepare_for_module TGT)
    target_compile_options(${TGT} PUBLIC -fmodules-ts)
    set_property(TARGET ${TGT} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${TGT} PROPERTY CXX_EXTENSIONS OFF)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_dependencies(${TGT} std_modules)
    endif()
endfunction()

set (MODULE_EXT cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set (MODULE_EXT ixx)
    add_compile_options(/utf-8)
endif()

add_executable(
        ${TARGET}
        src/core/Concepts.${MODULE_EXT}
        src/core/Core.${MODULE_EXT}
        src/task/AsyncTaskQueue.${MODULE_EXT}
        src/task/Coroutine.${MODULE_EXT}
        src/task/AsyncTaskLoop.${MODULE_EXT}
        src/task/Asyncify.${MODULE_EXT}
        src/task/Task.${MODULE_EXT}
        src/io/AsyncIoTask.${MODULE_EXT}
        src/io/AsyncIoLoop.${MODULE_EXT}
        src/io/AsyncIoAsyncify.${MODULE_EXT}
        src/io/Io.${MODULE_EXT}
        src/fs/FileSystem.${MODULE_EXT}
        src/main.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    prepare_for_module(${TARGET})
endif()