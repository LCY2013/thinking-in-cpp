cmake_minimum_required(VERSION 3.22)

project(modules)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
        MyAppV1
        src/includev1/a.cpp
        src/includev1/b.cpp
)

add_executable(
        MyAppV2
        src/includev2/a.cpp
        src/includev2/b.cpp
)
target_include_directories(
        MyAppV2
        PRIVATE
        src/includev2
)
target_sources(
        MyAppV2
        PRIVATE
        src/includev2/b.h
)

add_executable(
        MyAppV3
        src/includev3/a.cpp
        src/includev3/b.cpp
)
target_include_directories(
        MyAppV3
        PRIVATE
        src/includev3
)
target_sources(
        MyAppV3
        PRIVATE
        src/includev3/b.h
)