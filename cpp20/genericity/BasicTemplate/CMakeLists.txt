cmake_minimum_required(VERSION 3.22)

set (TARGET BasicTemplate)

project(${TARGET})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

add_executable(
        ${TARGET}
        src/main.cpp
        src/c11.cpp
)
target_include_directories(
        ${TARGET}
        PRIVATE
        src
)
target_sources(
        ${TARGET}
        PRIVATE
        src/c11.h
)