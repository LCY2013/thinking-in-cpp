cmake_minimum_required(VERSION 3.16)

set (TARGET ErrorMessage)

project(${TARGET})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

add_executable(
        ${TARGET}
        src/main.cpp
)